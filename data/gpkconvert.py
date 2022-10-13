from enum import Enum

import os

class FileMode(Enum):
    READ = 0;
    WRITE = 1;

class BaseHeader:
    isCompressed = False
    name = ""
    entries = 0

    def __init__(self, isCompressed, name, entries):
        self.isCompressed = isCompressed
        self.name = name
        self.entries = entries


class FileHeader:
    mountPoint = "" #128 Bytes
    name = "" #32 Bytes
    isCompressed = False #1 Byte
    size = 0 #4 Bytes

    def __init__(self, mountPoint, name, isCompressed, size):
        self.mountPoint = mountPoint
        self.name = name
        self.isCompressed = isCompressed
        self.size = size


class GPK:
    target = ""
    buffer = b""
    filemode = FileMode.READ;
    file = None;

    def __init__(self, target):
        self.target = target + ".gpk";

    def _openFile(self):
        file = None
        if self.filemode == FileMode.READ:
            s_filemode = "rb"
            #If file exists add contents to buffer
            file = open(self.target, "rb")
            self.buffer = file.read()
            file.close()

        if self.filemode == FileMode.WRITE: #Read file into buffer first
            if os.path.isfile(self.target) and os.stat(self.target).st_size > 0:
                #If file exists add contents to buffer
                file = open(self.target, "rb")
                self.buffer = file.read()
                file.close()
                file = open(self.target, "wb")
 
            else:
 
                file = open(self.target, "wb")
                baseHeader = BaseHeader
                baseHeader.isCompressed = True
                baseHeader.name = bytes("Test".encode('ascii')) + b"    "
                baseHeader.entries = 0
                self.buffer = b"GPAK" + bytes(baseHeader.isCompressed) + bytes(baseHeader.name) + baseHeader.entries.to_bytes(4, byteorder='little')
                LenFileTable = 0x1000
                self.buffer += b"\0" * LenFileTable
        return file

    def _closeFile(self, file):
        file.write(self.buffer)
        file.close()


    def addFileEntry(self, mountpoint, filename, isCompressed):

        self.filemode = FileMode.WRITE
        outFile = self._openFile()
        inFile = open(mountpoint + "\\" + filename, "rb")
        inBuffer = inFile.read()
        #File Header
        fileHeader = FileHeader(bytes(mountpoint, encoding='ASCII'), bytes(filename, encoding='ASCII'), False, len(inBuffer))

        #Bin format
        padMountPoint = b"\x00" * (128 - len(fileHeader.mountPoint) - 2)
        padName = b"\x00" * (32 - len(fileHeader.name))
        binFileHeader = fileHeader.mountPoint + bytes("\\a", encoding='ASCII') + padMountPoint + fileHeader.name + padName + bytes(fileHeader.isCompressed) + fileHeader.size.to_bytes(4, byteorder='little')


        #Find last file entry
        #Load file table into memory


        OffFileTable = 0x11
        LenFileTable = 0x1000

        self.buffer = bytearray(self.buffer)

        fileTable = self.buffer[OffFileTable:OffFileTable+LenFileTable]

        for i in range(0, len(fileTable), 4):
            print(i)
            if fileTable[i:i+4] == b'\x00\x00\x00\x00':
                #Found empty slot
                #Find offset of previous entry
                if i >= 4:
                    prevOffset = fileTable[i-4:i]
                    prevOffset = int.from_bytes(prevOffset, "little")
                    print("PREVOFF", prevOffset)
                    prevSize = self.buffer[prevOffset + 0xA0:prevOffset + 0xA4]
                    print("PREVSIZE", prevSize)
                    prevSize = int.from_bytes(prevSize, byteorder='little')

                    newOffset = prevOffset + prevSize + len(binFileHeader)
                    print(newOffset)
                else:
                    newOffset = (len(self.buffer)+1)
                    self.buffer += b"\x00" * (newOffset + fileHeader.size)
                bNewOffset = newOffset.to_bytes(4, byteorder="little")
                fileTable[i:i+4] = bNewOffset
                #Add File Header
                self.buffer[OffFileTable:OffFileTable+LenFileTable] = fileTable
                self.buffer[newOffset:newOffset + int(len(binFileHeader))] = binFileHeader
                self.buffer[newOffset + len(binFileHeader):newOffset + len(binFileHeader)+len(inBuffer)] = inBuffer
                break
        
        self._closeFile(outFile)
        

                


def main():
    gpk = GPK("test")
    if(os.path.isfile('test.gpk')):
        os.remove("test.gpk")
    gpk.filemode = FileMode.WRITE

    for root, dirs, files in os.walk("root", topdown=False):
        for dir in files:
            gpk.addFileEntry(root, dir, False)


if __name__ == "__main__":
    main()