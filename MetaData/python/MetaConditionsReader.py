import os, json
import collections
import jsonschema

class MetaConditionsReader(collections.MutableMapping):

    def __init__(self, json_file_names):
        """
        Define required fields as a jsonschema (https://json-schema.org/understanding-json-schema/reference/object.html)
        """

        self.store = dict()
        
        # define schema
        self.required_fields = {
            "type" : "object",
            "properties":
            {
                "globalTags" :
                {
                    "type": "object",
                    "properties" :
                    {
                        "data" : {"type": "string"},
                        "MC" : {"type": "string"},
                    }
                },
                "flashggMETsFunction": {"type": "string"},
	            	"DeepJet": {"type": "string"},
            },
            "additionalProperties" : True
        }
        # make all defined properties required
        def makeRequired(obj):
            obj["required"] = []
            for field in obj["properties"]:
                obj["required"].append(field)
                if obj["properties"][field]["type"]  == "object":
                    makeRequired(obj["properties"][field])

        makeRequired(self.required_fields)
                    
        if len(json_file_names) > 0:
            if isinstance(json_file_names, basestring):
                with open(os.path.expandvars(json_file_names)) as json_file:
                    self.store = json.load(json_file)
            else:
                for json_file_name in json_file_names:
                    with open(os.path.expandvars(json_file_name)) as json_file:
                        self.store.update(json.load(json_file))
        else:
            raise Exception, ">>> MetaConditionsReader: JSON file name is an empty string."

        try:
            jsonschema.validate(self.store, self.required_fields)
        except jsonschema.exceptions.ValidationError as ve:
            raise Exception, 'Input data validation \n error: %s' % ve
        
    def __getitem__(self, key):
        return self.store[self.__keytransform__(key)]

    def __setitem__(self, key, value):
        self.store[self.__keytransform__(key)] = value

    def __delitem__(self, key):
        del self.store[self.__keytransform__(key)]

    def __iter__(self):
        return iter(self.store)
        
    def __len__(self):
        return len(self.store)

    def __keytransform__(self, key):
        return key
            
                
