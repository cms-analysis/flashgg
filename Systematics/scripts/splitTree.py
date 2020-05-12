import tree2dataset as t2d
import ROOT
import root_pandas
from collections import OrderedDict
import argparse
import os
import oyaml as yaml


def parse_variables(variables):
    return [(key, float(variables[key][0]), float(variables[key][1])) for key in variables.keys()]


def write_cat_procs_file(path, procs, cats):
    proc_str = ''
    for proc in procs:
        proc_str += '{},'.format(proc)
    proc_str = proc_str[:-1] + '\n'
    cat_str = ''
    for cat in cats:
        cat_str += '{},'.format(cat)
    cat_str = cat_str[:-1]
    with open(path, 'w') as fil:
        fil.write(proc_str)
        fil.write(cat_str)


def evaluate_formulas(df, form_dic):
    for form in form_dic.keys():
        df[form] = df.eval(form_dic[form], engine='python')


def main(options):

    outfolder = os.path.abspath(options.outfolder)
    if not os.path.exists(outfolder):
        os.mkdir(outfolder)
    config = yaml.load(open(options.config))
    if options.process == 'Data' or 'OA' in options.process:
        for key in config['splits'].keys():
            if 'gen' in key:
                del config['splits'][key]
    print(config['splits'])
    splitDic = OrderedDict(config['splits'])
    if options.process == 'Data':
        varDic = {'CMS_hgg_mass': [100, 180],
                  'weight': ['-inf', 'inf'], 'lumi': ['-inf', 'inf']}
    else:
        varDic = config['variables']
    print(varDic)
    variables = parse_variables(varDic)
    print(variables)
    cut = config['cut']
    if options.process == 'Data':
        weight = 'weight'
    else:
        weight = config['weight']
    outfile = config['filenames'][options.process]
    label = options.label
    if 'replace' in config.keys():
        replace = config['replace']
    else:
        replace = None

    systVars = config['systVars']
    for var in systVars:
        for dirr in ['Up01sigma', 'Down01sigma']:
            variables.append(('{}{}'.format(var, dirr), -1000, 1000))

    procs = []
    cats = []
    ws = ROOT.RooWorkspace('cms_hgg_13TeV')
    for cat in config['categories']:

        proc = config['procs'][options.process]

        if 'formulas' in config.keys():
            columns = None
        else:
            columns = splitDic.keys() + [var[0]
                                         for var in variables] + ['leadmva', 'subleadmva']

        df = root_pandas.read_root(
            options.infile, '{}/{}_{}'.format(config['treepath'], proc, cat), columns=columns)

        if 'formulas' in config.keys():
            evaluate_formulas(df, config['formulas'])

        df.query(cut, engine='python', inplace=True)

        if label is not None:
            cat += '_{}'.format(label)
        if replace is not None:
            proc = proc.replace(replace[0], replace[1])

        w = t2d.RooWorkspaceFromDataframe(
            df, splitDic, variables, weight, "cms_hgg_13TeV", (proc, cat), ws)
        w.makeCategories()
        w.makeWorkspace()

        if len(w.labels) > 1 and all('gen' in la for la in w.labels[0]):
            procs_temp = ['{}_{}'.format(proc, lab) for lab in w.labels[0]]
            cats_temp = []
            for labs in w.labels[1:]:
                if len(cats_temp) == 0:
                    cats_temp = ['{}'.format(lab) for lab in labs]
                else:
                    cats_temp = ['{}_{}'.format(categ, lab)
                                 for lab in labs for categ in cats_temp]
            cats_temp = ['{}_{}'.format(categ, cat) for categ in cats_temp]
        else:
            procs_temp = ['{}'.format(proc)]
            cats_temp = ['{}_{}'.format(lab, cat) for lab in w.labels[0]]

        procs.extend([x for x in procs_temp if x not in procs])
        cats.extend([x for x in cats_temp if x not in cats])

        del df, w

    for mass in ['120', '125', '130']:
        try:
            procs[0].index(mass)
            procs = [pro.replace(mass, '') for pro in procs]
        except ValueError:
            pass

    procs = [pro.replace('__13TeV', '') for pro in procs]
    write_cat_procs_file(
        '{}/proc_cat_names_{}.txt'.format(outfolder, splitDic.keys()[0]), procs, cats)

    f = ROOT.TFile('{}/{}'.format(outfolder, outfile), "RECREATE")
    ws.Write("cms_hgg_13TeV")
    f.Write()


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    requiredAgrs = parser.add_argument_group()
    requiredAgrs.add_argument(
        '--infile', '-i', action='store', type=str, required=True)
    requiredAgrs.add_argument(
        '--process', '-p', action='store', type=str, required=True)
    requiredAgrs.add_argument(
        '--outfolder', '-o', action='store', type=str, required=True)
    requiredAgrs.add_argument(
        '--config', '-c', action='store', type=str, required=True)
    optionalArgs = parser.add_argument_group()
    optionalArgs.add_argument('--label', '-l', action='store', type=str)
    options = parser.parse_args()
    main(options)
