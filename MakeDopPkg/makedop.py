import os
import xml.etree.ElementTree as ET


class MakeDop:
    def __init__(self, dir="in"):
        self.dir = dir
        self.metafile = ""
        self.metadata = {
            "name": ""
        }

    def findMetadataFiles(self):
        for filename in os.listdir(self.dir):
            f = os.path.join(self.dir, filename)
            # checking if it is a file
            if os.path.isfile(f):
                if os.path.splitext(f)[1] == ".xml":
                    self.metafile = f

    def readMetadata(self):
        root = ET.parse(self.metafile).getroot()
        self.metadata["name"] = root.get('name')


if __name__ == '__main__':
    makeDop = MakeDop()
    makeDop.findMetadataFiles()
    makeDop.readMetadata()
    print("a")