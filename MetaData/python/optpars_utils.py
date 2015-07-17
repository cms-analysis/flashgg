from optparse import OptionParser, make_option

import json
import csv

# -----------------------------------------------------------------------------------------------------------
class ScratchAppend:
    def __init__(self,typ=str):
        self.cold = True
        self.typ = typ
        
    def __call__(self,option, opt_str, value, parser, *args, **kwargs):
        target = getattr(parser.values,option.dest)
        if self.cold:
            del target[:]
            self.cold = False
        print value
        if type(value) == str and "," in value:
            for v in value.split(","):
                target.append(self.typ(v))
        else:
            target.append(self.typ(value))
                                                    
# -----------------------------------------------------------------------------
class Load:
    def __call__(self,option, opt_str, value, parser, *args, **kwargs):
        if option.dest == "__opts__":
            dest = parser.values
        else:
            dest = getattr(parser.values,option.dest)
        origin = getattr(parser.values,"%s_src" % option.dest,None)
        if origin:
            origin += ",%s" % value
        else:
            origin = value
            setattr(parser.values,"%s_src" % option.dest,origin)

        if type(dest) == dict:
            setter = dict.__setitem__
            getter = dict.get
        else:
            setter = setattr
            getter = getattr
        
        for cfg in value.split(","):
            cf = open(cfg)
            settings = json.loads(cf.read())
            for k,v in settings.iteritems():
                attr  = getter(dest,k,None)
                if attr and type(attr) == list:           
                    attr.extend(v)
                setter(dest,k,v)
            cf.close()

# -----------------------------------------------------------------------------
class Csv:
    def __call__(self,option, opt_str, value, parser, *args, **kwargs):
        dest = getattr(parser.values,option.dest)
        if not dest:
            setattr(parser.values,option.dest,[])
            dest = getattr(parser.values,option.dest)
        cf = open(value)
        reader = csv.DictReader(cf)
        for row in reader:
            dest.append(row)            
        cf.close()
        
