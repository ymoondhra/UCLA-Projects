#NAME: Shubhrose Singh, Yash Moondhra
#EMAIL: shubhroses@gmail.com, ymoondhra@gmail.com
#ID: 987654321, 123456789 

import csv, os, sys

super_block = None
group = None
inodes = []
free_blocks = set()
free_inodes = set()
indirects = []
dirents = []
inode_nums = set()
used_blocks = {}

consistent = True

class Superblock:
    def __init__(self, row):
        self.block_count = int(row[1])
        self.inode_count = int(row[2])
        self.block_size= int(row[3])
        self.inode_size= int(row[4])
        self.blocks_per_group =int(row[5])
        self.inodes_per_group =int(row[6])
        self.first_inode_available = int(row[7])
        global MAX_NUM_BLOCKS
        MAX_NUM_BLOCKS = self.block_count

class Group:
    def __init__(self, row):
        self.group_number     = int(row[1])
        self.total_group_count= int(row[2])
        self.inodes_per_group = int(row[3])
        self.free_block_count = int(row[4])
        self.free_inode_count = int(row[5])
        self.first_inode_block = int(row[8])
        self.inode_table      = int(row[-1])

class Dirent:
    def __init__(self, row):
        self.parent_inode_num  =int(row[1])
        self.logical_byte_offset  =int(row[2])
        self.inode_num             =int(row[3])
        self.entry_length         =int(row[4])
        self.name_length          =int(row[5])
        self.name_of_file         =str(row[6])

class Indirect:
    def __init__(self, row):
        self.inode_num                  =int(row[1])
        self.level                      =int(row[2])
        self.offset                     =int(row[3])
        self.block_num                  =int(row[4])
        self.block_num_referenced_block =int(row[5])

class Inode:
    def get_blocks(self, row):
        if row[2] == 'd' or row[2] == 'f':
            for i in range(12,27):
                self.blocks.append(int(row[i]))

    def __init__(self, row):
        self.inode_num = int(row[1])
        self.file_type = row[2]
        self.owner = int(row[4])
        self.group = int(row[5])
        self.link_count = int(row[6])
        self.last_changed = row[7]
        self.last_modified = row[8]
        self.last_access = row[9]
        self.file_size = int(row[10])
        self.num_blocks_used = int(row[11])
        self.blocks = []
        self.get_blocks(row)


def decipher_csv(csv_file):
    reader = csv.reader(csv_file)
    for row in reader:
        if row[0] == "SUPERBLOCK":
            global super_block
            super_block = Superblock(row)
        if row[0] == "GROUP":
            global group
            group = Group(row)
        if row[0] == "DIRENT":
            dirents.append(Dirent(row))
        if row[0] == "INDIRECT":
            global indirects
            indirects.append(Indirect(row))
            free_blocks.add(int(row[5]))
        if row[0] == "INODE":
            inodes.append(Inode(row))
        if row[0] == "IFREE":
            free_inodes.add(int(row[1]))
        if row[0] == "BFREE":
            free_blocks.add(int(row[1]))
            
def dir_consistency_audit():
    link_counts = {}
    global dirents
    for i in range (1, super_block.inode_count + 1):
        link_counts[i] = 0

    for dirent in dirents:
        isValidDir = True
        if dirent.inode_num < 1 or dirent.inode_num > super_block.inode_count:
            print "DIRECTORY INODE",dirent.parent_inode_num,"NAME",dirent.name_of_file,"INVALID INODE",dirent.inode_num
            isValidDir = False
            consistent = False
            break
        else:
            if dirent.parent_inode_num != dirent.inode_num:
                if dirent.name_of_file == "'.'":
                    print "DIRECTORY INODE",dirent.parent_inode_num,"NAME",dirent.name_of_file,"LINK TO INODE",dirent.inode_num,"SHOULD BE",dirent.parent_inode_num
                    consistent = False
                elif dirent.name_of_file == "'..'" and dirent.parent_inode_num == 2:
                    print "DIRECTORY INODE",dirent.parent_inode_num,"NAME",dirent.name_of_file,"LINK TO INODE",dirent.inode_num,"SHOULD BE",dirent.parent_inode_num
                    consistent = False
            if dirent.name_of_file == "'..'" and dirent.parent_inode_num != 2:
                for p in dirents:
                    if p.inode_num == dirent.parent_inode_num and p.parent_inode_num != dirent.parent_inode_num:
                        if dirent.inode_num != p.parent_inode_num:
                            print "DIRECTORY INODE",dirent.parent_inode_num,"NAME",dirent.name_of_file,"LINK TO INODE",dirent.inode_num,"SHOULD BE",p.parent_inode_num,
                            consistent = False
            
        isInDir = False
        for inode in inodes:
            if dirent.inode_num == inode.inode_num:
                isInDir = True
                consistent = False
                break

        if isInDir == False:
            print "DIRECTORY INODE",dirent.parent_inode_num,"NAME",dirent.name_of_file,"UNALLOCATED INODE",dirent.inode_num
            consistent = False

        if isValidDir and dirent.inode_num in link_counts:
            link_counts[dirent.inode_num] += 1

    for inode in inodes:
        if inode.inode_num in link_counts:
            if inode.link_count != link_counts[inode.inode_num]:
                print "INODE",inode.inode_num,"HAS",link_counts[inode.inode_num],"LINKS BUT LINKCOUNT IS",inode.link_count
                consistent = False


def block_consistency_audit():
    for inode in inodes:
        for i in range(0, len(inode.blocks)):
            currentBlock = inode.blocks[i]
            #check if valid                                                                                                                             
            offset = 0
            level = 0
            if (i == 12):
                offset = 12
                level = 1
            elif (i == 13):
                offset = 12 + 256
                level = 2
            elif (i == 14):
                offset = 12 + 256 + 256*256
                level = 3
            if(currentBlock < 0 or currentBlock > MAX_NUM_BLOCKS):
                if (level == 0):
                    print "INVALID BLOCK",currentBlock,"IN INODE",inode.inode_num,"AT OFFSET",offset
                if (level == 1):
                    print "INVALID INDIRECT BLOCK",currentBlock,"IN INODE",inode.inode_num,"AT OFFSET",offset
                if (level == 2):
                    print "INVALID DOUBLE INDIRECT BLOCK",currentBlock,"IN INODE",inode.inode_num,"AT OFFSET",offset
                if (level == 3):
                    print "INVALID TRIPLE INDIRECT BLOCK",currentBlock,"IN INODE",inode.inode_num,"AT OFFSET",offset
                consistent = False
            elif(currentBlock != 0):
                if(currentBlock < 8):
                    if (level == 0):
                        print "RESERVED BLOCK",currentBlock,"IN INODE",inode.inode_num,"AT OFFSET",offset
                    if (level == 1):
                        print "RESERVED INDIRECT BLOCK",currentBlock,"IN INODE",inode.inode_num,"AT OFFSET",offset
                    if (level == 2):
                        print "RESERVED DOUBLE INDIRECT BLOCK",currentBlock,"IN INODE",inode.inode_num,"AT OFFSET",offset
                    if (level == 3):
                        print "RESERVED TRIPLE INDIRECT BLOCK",currentBlock,"IN INODE",inode.inode_num,"AT OFFSET",offset
                    consistent = False
                else:
                    block_info = [currentBlock, inode.inode_num, offset, level]
                    existing_block_info = used_blocks.get(currentBlock)
                    if  (existing_block_info == None):
                        used_blocks[currentBlock] = block_info
                    else:
                        if(level == 0):
                            print "DUPLICATE BLOCK",currentBlock,"IN INODE",inode.inode_num,"AT OFFSET",offset
                        if(level == 1):
                            print "DUPLICATE INDIRECT BLOCK",currentBlock,"IN INODE",inode.inode_num,"AT OFFSET",offset
                        if(level == 2):
                            print "DUPLICATE DOUBLE INDIRECT BLOCK",currentBlock,"IN INODE",inode.inode_num,"AT OFFSET",offset
                        if(level == 3):
                            print "DUPLICATE TRIPLE INDIRECT BLOCK",currentBlock,"IN INODE",inode.inode_num,"AT OFFSET",offset

                        if(existing_block_info[3] == 0):
                            print "DUPLICATE BLOCK",existing_block_info[0],"IN INODE",existing_block_info[1],"AT OFFSET",existing_block_info[2]
                        if(existing_block_info[3] == 1):
                            print "DUPLICATE INDIRECT BLOCK",existing_block_info[0],"IN INODE",existing_block_info[1],"AT OFFSET",existing_block_info[2]
                        if(existing_block_info[3] == 2):
                            print "DUPLICATE DOUBLE INDIRECT BLOCK",existing_block_info[0],"IN INODE",existing_block_info[1],"AT OFFSET",existing_block_info[2]
                        if(existing_block_info[3] == 3):
                            print "DUPLICATE TRIPLE INDIRECT BLOCK",existing_block_info[0],"IN INODE",existing_block_info[1],"AT OFFSET",existing_block_info[2]
                        consistent = False
    
    for indirect in indirects:
        if(indirect.block_num_referenced_block < 0 or indirect.block_num_referenced_block > MAX_NUM_BLOCKS):
            if (indirect.level == 0):
                print "INVALID BLOCK",indirect.block_num_referenced_block,"IN INODE",indirect.inode_num,"AT OFFSET",indirect.offset
            if (indirect.level == 1):
                print "INVALID INDIRECT BLOCK",indirect.block_num_referenced_block,"IN INODE",indirect.inode_num,"AT OFFSET",indirect.offset
            if (indirect.level == 2):
                print "INVALID DOUBLE INDIRECT BLOCK",indirect.block_num_referenced_block,"IN INODE",indirect.inode_num,"AT OFFSET",indirect.offset
            if (indirect.level == 3):
                print "INVALID TRIPLE INDIRECT BLOCK",indirect.block_num_referenced_block,"IN INODE",indirect.inode_num,"AT OFFSET",indirect.offset
            consistent = False
        elif(indirect.block_num_referenced_block != 0):
            if(indirect.block_num_referenced_block < 8):
                if (indirect.level == 0):
                    print "RESERVED BLOCK",indirect.block_num_referenced_block,"IN INODE",indirect.inode_num,"AT OFFSET",indirect.offset
                if (indirect.level == 1):
                    print "RESERVED INDIRECT BLOCK",indirect.block_num_referenced_block,"IN INODE",indirect.inode_num,"AT OFFSET",indirect.offset
                if (indirect.level == 2):
                    print "RESERVED DOUBLE INDIRECT BLOCK",indirect.block_num_referenced_block,"IN INODE",indirect.inode_num,"AT OFFSET",indirect.offset
                if (indirect.level == 3):
                    print "RESERVED TRIPLE INDIRECT BLOCK",indirect.block_num_referenced_block,"IN INODE",indirect.inode_num,"AT OFFSET",indirect.offset
                consistent = False

    global super_block
    global group
    block_start = int(super_block.inode_size * super_block.inode_count / super_block.block_size) + group.first_inode_block

    for i in range (block_start, super_block.block_count):
        isFreelist = False
        if i in free_blocks:
            isFreelist = True
        if isFreelist and used_blocks.get(i) != None:
            print "ALLOCATED BLOCK",i,"ON FREELIST"
            consistent = False

    for inode in inodes:
        if inode.inode_num in free_inodes:
            print "ALLOCATED INODE",inode.inode_num,"ON FREELIST"
            consistent = False

    for i in range(super_block.first_inode_available, super_block.inode_count):
        isFreelist = False
        if i not in free_inodes:
            isFreelist = True
        if isFreelist and i not in inode_nums:
            print "UNALLOCATED INODE",i,"NOT ON FREELIST"

    for i in range (block_start, super_block.block_count):
        isFreelist = False
        if i not in free_blocks:
            isFreelist = True
        if isFreelist and used_blocks.get(i) == None:
            print "UNREFERENCED BLOCK %d" %(i)
            consistent = False

    dir_consistency_audit()

def main():
    if len(sys.argv) != 2:
        sys.stderr.write("Error: input one .csv file")
        exit(1)
    try:
        csv_file = open(sys.argv[1])
    except:
        sys.stderr.write("Error: could not open csv file")
        exit(1)
    decipher_csv(csv_file)

    for i in range(len(inodes)):
        inode_nums.add(inodes[i].inode_num)
    block_consistency_audit()
    
    if(consistent):
        sys.exit(0)
    sys.exit(2)


if __name__ == '__main__':
    main()
