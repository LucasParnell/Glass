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
    isCompressed = 0 #1 Byte
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
    gpkName = ""

    def __init__(self, target, gpkName):
        self.target = target + ".gpk";
        self.gpkName = gpkName

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
                baseHeader.name = bytes(self.gpkName, encoding='ASCII');
                padName = b"\x00" * (8 - len(baseHeader.name))
                baseHeader.entries = 0 #Unused
                self.buffer = b"GPAK" + baseHeader.isCompressed.to_bytes(1, byteorder="little") + baseHeader.name + padName + baseHeader.entries.to_bytes(4, byteorder='little')
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
        fileHeader = FileHeader(bytes(mountpoint + "\\", encoding='ASCII'), bytes(filename, encoding='ASCII'), 0, len(inBuffer))

        #Bin format
        padMountPoint = b"\x00" * (128 - len(fileHeader.mountPoint))
        padName = b"\x00" * (32 - len(fileHeader.name))
        binFileHeader = fileHeader.mountPoint + padMountPoint + fileHeader.name + padName + fileHeader.isCompressed.to_bytes(1, byteorder='little') + fileHeader.size.to_bytes(4, byteorder='little')


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
                    #Get Last Offset
                    prevOffset = fileTable[i-4:i]
                    prevOffset = int.from_bytes(prevOffset, "little")
                    print("PREVOFF", prevOffset)
                    #Size is 0xA1 to 0xA5
                    prevSize = self.buffer[prevOffset + 0xA1:prevOffset + 0xA5]
                    print("PREVSIZE", prevSize)
                    prevSize = int.from_bytes(prevSize, byteorder='little')

                    newOffset = prevOffset + prevSize + len(binFileHeader)
                    print(newOffset)
                else:
                    newOffset = (len(self.buffer)+1)
                    print("BUFFER SIZE: " + str(len(self.buffer)))
                    print("NEW OFFSET: " + str(newOffset))
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
    gpk = GPK("test", "GamePak")
    if(os.path.isfile('test.gpk')):
        os.remove("test.gpk")
    gpk.filemode = FileMode.WRITE

    for root, dirs, files in os.walk("root", topdown=False):
        for dir in files:
            gpk.addFileEntry(root, dir, False)


if __name__ == "__main__":
    main()