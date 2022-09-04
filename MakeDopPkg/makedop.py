import os
import xml.etree.ElementTree as ET
from collections import defaultdict
import json



def etree_to_dict(t):
    d = {t.tag: {} if t.attrib else None}
    children = list(t)
    if children:
        dd = defaultdict(list)
        for dc in map(etree_to_dict, children):
            for k, v in dc.items():
                dd[k].append(v)
        d = {t.tag: {k: v[0] if len(v) == 1 else v
                     for k, v in dd.items()}}
    if t.attrib:
        d[t.tag].update(('@' + k, v)
                        for k, v in t.attrib.items())
    if t.text:
        text = t.text.strip()
        if children or t.attrib:
            if text:
                d[t.tag]['#text'] = text
        else:
            d[t.tag] = text
    return d


def convertObj(filename):
    buf = b""
    #Get vertices, indices and texcoords for now
    file = open(filename, "r")
    contents = file.readlines()
    vertices = []
    texcoords = []
    normals = []
    indices = []
    for line in contents:
        if "v " in line: #Vertices
            line = line[2:].strip("\n").split(" ")
            vertices += line
        elif "vt" in line:
            line = line[3:].strip("\n").split(" ")
            texcoords += line
        elif "vn" in line:
            line = line[3:].strip("\n").split(" ")
            normals += line
        elif "f" in line:
            ind = line[2:].strip("\n").split(" ")
            buff = []
            for i in ind:
                ind = i.split("/")
                buff += ind
            indices += buff

    print(vertices)
    buf += len(vertices).to_bytes(32, "big")
    buf += bytes("\x00".join(vertices), "ascii")
    buf += len(texcoords).to_bytes(32, "big")
    buf += bytes("\x00".join(texcoords), "ascii")
    buf += len(normals).to_bytes(32, "big")
    buf += bytes("\x00".join(normals), "ascii")
    buf += len(indices).to_bytes(32, "big")
    buf += bytes("\x00".join(indices), "ascii")
    return buf

class MakeDop:
    def __init__(self, dir="in"):
        self.dir = dir
        self.metafile = ""
        self.metadata = {
        }

        self.name = ""
        self.entities = []


    def findMetadataFiles(self):
        for filename in os.listdir(self.dir):
            f = os.path.join(self.dir, filename)
            # checking if it is a file
            if os.path.isfile(f):
                if os.path.splitext(f)[1] == ".xml":
                    self.metafile = f

    def readMetadata(self):
        root = ET.parse(self.metafile).getroot()
        self.metadata = etree_to_dict(root)

        self.name = self.metadata["Dop"]["@name"]
        for i in self.metadata["Dop"]["Entities"]["Prop"]:
            self.entities.append(i)

    def writeDop(self):
        try:
            file = open("out/"+self.name+".dop", "xb")
        except(FileExistsError):
            os.remove("out/"+self.name+".dop")
            file = open("out/" + self.name + ".dop", "xb")

        buffer = b""
        buffer += bytes(self.name, "ascii") #Name
        buffer += len(self.entities).to_bytes(32, byteorder='big') #NumProps
        #Begin prop list
        for i in self.entities:
            string = json.dumps(i)
            buffer += len(string).to_bytes(32, byteorder='big')
            buffer += bytes(string, "ascii")
        buffer += bytes("END", "ascii") #Header finished, now the annoying part

        #Reading referenced files
        filestoadd = []
        for i in self.entities:
                for x in i["Components"]:
                    if x == "Mesh":
                        print(i["Components"]["Mesh"])
                        if i["Components"]["Mesh"]["@at"] not in filestoadd:
                            filestoadd.append(i["Components"]["Mesh"]["@at"])

        for i in filestoadd:
            #Check if is obj file
            if ".obj" in i:
                #Convert_obj
                objbuf = convertObj(self.dir+"/"+i)

        buffer += objbuf
        file.write(buffer)


if __name__ == '__main__':
    makeDop = MakeDop()
    makeDop.findMetadataFiles()
    makeDop.readMetadata()
    #makeDop.process()
    makeDop.writeDop()
