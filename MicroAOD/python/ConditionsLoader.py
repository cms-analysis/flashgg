import os, json

def loadConditions(json_file_name):
    """
    Just a wrapper around json.load_data()
    """

    if json_file_name != '':
        with open(json_file_name) as json_file:
            return json.load(json_file)
    else:
        return {}
