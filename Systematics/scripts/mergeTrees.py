import root_pandas
import argparse


def main(options):
    dfs = []
    for tag in range(options.nTags):
        try:
            dfs.append(root_pandas.read_root(options.infile, '{}_{}'.format(options.treename, tag)))
        except IOError:
            pass

    df_out = dfs[0]
    for df in dfs[1:]:
        df_out = df_out.append(df)

    df_out.to_root(options.outfile, key=options.treename + '_0')

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    requiredArgs = parser.add_argument_group()
    requiredArgs.add_argument('-i', '--infile', action='store', type=str, required=True)
    requiredArgs.add_argument('-o', '--outfile', action='store', type=str, required=True)
    requiredArgs.add_argument('-t', '--treename', action='store', type=str, required=True)
    requiredArgs.add_argument('-N', '--nTags', action='store', type=int, required=True)
    options = parser.parse_args()
    main(options)
