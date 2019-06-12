//NAME: Shubhrose Singh, Yash Moondhra
//EMAIL: shubhroses@gmail.com, ymoondhra@gmail.com
//ID: 987654321, 123456789

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/types.h>
#include <math.h>
#include <sys/stat.h>
#include <inttypes.h>
#include "ext2_fs.h"

int image_fd;
struct ext2_super_block superBlock;
__u32 bSize; //blocksize
struct ext2_group_desc* groupDescriptors;
int numGroups;


void setupArgs(int argc, char **argv) {
  if(argc != 2) {
    fprintf(stderr, "Error: You must provide exactly one argument, which is the disk image name\n");
    exit(1);
  }

  image_fd = open(argv[1], O_RDONLY);
  if(image_fd < 0) {
    fprintf(stderr, "Error: Could not open disk image\n");
    exit(1);
  }
}

void superBlockInfo() {
  int readSuperBlock = pread(image_fd, &superBlock, sizeof(struct ext2_super_block), 1024);
  if(readSuperBlock == -1) {
    fprintf(stderr, "Error: could not read superblock");
    exit(1);
  }
  bSize = EXT2_MIN_BLOCK_SIZE << superBlock.s_log_block_size;
  fprintf(stdout, "SUPERBLOCK,%u,%u,%u,%u,%u,%u,%u\n", superBlock.s_blocks_count, superBlock.s_inodes_count, bSize, superBlock.s_inode_size, superBlock.s_blocks_per_group, superBlock.s_inodes_per_group, superBlock.s_first_ino);
      
}

void groupInfo() {
  numGroups = 1 + (superBlock.s_blocks_count) / superBlock.s_blocks_per_group;
  
  groupDescriptors = (struct ext2_group_desc*) malloc(numGroups * sizeof(struct ext2_group_desc));
  if(groupDescriptors == NULL) {
    fprintf(stderr, "Error: could not dynamically allocate array for group descriptors\n");
    exit(1);
  }
  int i = 0;
  for(; i < numGroups; i++) {
    int bSizeInt = (int) bSize;
    int offset = 1024 + bSizeInt + (i * sizeof(struct ext2_group_desc));
    
    int numBlocksInGroup = superBlock.s_blocks_count;
    if(superBlock.s_blocks_per_group <= superBlock.s_blocks_count) {
      numBlocksInGroup = superBlock.s_blocks_per_group;
    }
    
    if(pread(image_fd, &groupDescriptors[i], sizeof(struct ext2_group_desc), offset) == -1) {
      fprintf(stderr, "Error: could not read from group %d\n", i);
      exit(1);
    }
    fprintf(stdout, "GROUP,%d,%u,%u,%u,%u,%u,%u,%u\n", i, numBlocksInGroup, superBlock.s_inodes_per_group, groupDescriptors[i].bg_free_blocks_count, groupDescriptors[i].bg_free_inodes_count, groupDescriptors[i].bg_block_bitmap, groupDescriptors[i].bg_inode_bitmap, groupDescriptors[i].bg_inode_table); 
  }
}

void freeInfo() {
  int mask = 1;
  
  //BLOCK BITMAP
  int j = 0;
  int numBlocks = superBlock.s_blocks_per_group;
  for(; j < numGroups; j++) {
    __u32 blockNum = groupDescriptors[j].bg_block_bitmap;
    __u32 inodeNum = groupDescriptors[j].bg_inode_bitmap;
    
    int bSizeInt = (int) bSize;
    
    char* groupBitmap = malloc(bSizeInt);
    char* inodeBitmap = malloc(bSizeInt);

    int blockOffset = blockNum * bSizeInt;
    int inodeOffset = inodeNum * bSizeInt;

    int readGroup = pread(image_fd, groupBitmap, bSizeInt, blockOffset);
    int readinode = pread(image_fd, inodeBitmap, bSizeInt, inodeOffset);
    if(readGroup < 0 || readinode < 0) {
      fprintf(stderr, "Error: could not read the group bitmap for group %d", j);
      exit(1);
    }

    int k = 0;
    for(; k < bSizeInt; k++) {
      char groupByte = groupBitmap[k];
      char inodeByte = inodeBitmap[k];
      int x = 0;
      mask = 1;
      for(; x < 8; x++) {
	int groupBit = groupByte & mask;
	int inodeBit = inodeByte & mask;
	
	
	int freeBlockNum = x + 1 + j * numBlocks + k * 8;
	if(groupBit == 0) {
	  fprintf(stdout, "BFREE,%u\n", freeBlockNum);
	}

	int freeInodeNum = x + 1 + j * numBlocks + k * 8;
	if(inodeBit == 0) {
	  fprintf(stdout, "IFREE,%u\n", freeInodeNum);
	}

	mask = mask << 1;
      }
    }
    free(groupBitmap);
    free(inodeBitmap);
  }
}


void printingIndirectReferences(__u32 blockVal, int level, int numInode, int offset, __u32 *indirectElement)
{
  
  int indirectBlockOffset = bSize*blockVal;
  int indirectBlockRead = pread(image_fd, indirectElement, bSize,indirectBlockOffset);
  if(indirectBlockRead == -1)
    {
      fprintf(stderr, "Error: Could not read from indirect block");
      exit(1);
    }
  unsigned w = 0;
  while(w < bSize/4)
    {
      if(indirectElement[w] != 0)
	{
	  fprintf(stdout, "INDIRECT,%d,%d,%d,%u,%u\n",
		  numInode, level, offset, blockVal, indirectElement[w]);
	  if(level == 2)
	    {
	      __u32 *indirectElement2=malloc(bSize*sizeof(__u32));
	      printingIndirectReferences(indirectElement[w], 1, numInode, offset, indirectElement2);
	    }
	  else if(level ==3)
	    {
	      __u32 *indirectElement3=malloc(bSize*sizeof(__u32));
	      printingIndirectReferences(indirectElement[w], 2, numInode, offset, indirectElement3);
	    }
	}
      switch(level)
	{
	case 1: offset = offset + 1; break;
	case 2: offset += 16*16; break;
	case 3: offset += 65536; break;
	}
      w++;
    }
}


void inodeInfo()
{  
  int inodesPerGroup = (int) superBlock.s_inodes_per_group;
  int inodeTableSize = (int) inodesPerGroup * (int) superBlock.s_inode_size;
  struct ext2_inode* inodeTable = malloc(inodeTableSize* sizeof(char));
  if(inodeTable == NULL) 
    {
      fprintf(stderr, "Error: could not dynamically allocate memory for inode table representation\n");
      exit(1);
    }

  int i = 0;
  int bSizeInt = (int) bSize;
  while(i < numGroups) 
    {
      if(pread(image_fd, inodeTable, inodeTableSize, bSizeInt * groupDescriptors[i].bg_inode_table) == -1) 
	{
	  fprintf(stderr, "Error: could not read inode table");
	  exit(1);
	}
    
      int x = 0;
      for(; x < inodesPerGroup; x++) 
	{
	  struct ext2_inode* inode = &inodeTable[x];
	  char fileType = '?';
	  
	  if(inode->i_mode != 0 && inode->i_links_count != 0)  {
	    if(inode->i_mode & 0x8000 & 0x2000) {
	      fileType = 's';
	    }
	    else if((inode->i_mode & 0x8000) == 0x8000) {
	      fileType = 'f';
	    }
	    else if((inode->i_mode & 0x4000) == 0x4000) {
	      fileType = 'd';
	    }
	    
	    struct tm tiempo;

	    char createTime[50];
	    memset(createTime, 0, 50);
	    time_t c = inode->i_ctime;
	    tiempo = *gmtime(&c);
	    strftime(createTime, sizeof(createTime), "%D %T", &tiempo);
	    
	    char modTime[50];
	    memset(modTime, 0, 50);
	    time_t m = inode->i_mtime;
	    tiempo = *gmtime(&m);
	    strftime(modTime, sizeof(modTime), "%D %T", &tiempo);
	    
	    char accessTime[50];
	    memset(accessTime, 0, 50);
	    time_t a = inode->i_atime;	        
	    tiempo = *gmtime(&a);
	    strftime(accessTime, sizeof(accessTime), "%D %T", &tiempo);
                
	    fprintf(stdout, "INODE,%u,%c,%o,%u,%u,%u,%s,%s,%s,%u,%u", x+1, fileType, (inode->i_mode & 0xFFF), inode->i_uid, inode->i_gid, inode->i_links_count, createTime, modTime, accessTime, inode->i_size, inode->i_blocks);
	        
	    int q = 0;
	    while(q < 15) 
	      {
		__u32 numInodeBlock = inode->i_block[q];
		fprintf(stdout, ",%u", numInodeBlock);
		q++;
	      }
	    fprintf(stdout, "\n");
	        
	    if(fileType == 'd') 
	      {
		struct ext2_dir_entry* directoryEntry = malloc(sizeof(struct ext2_dir_entry));
		unsigned t = 0;
		while(t < EXT2_NDIR_BLOCKS)
		  {
		    int off = 0;
		    if(inode->i_block[t] != 0)
		      {
			while((unsigned) off < bSize) 
			  {
			    int directoryRead = pread(image_fd, directoryEntry, sizeof(struct ext2_dir_entry), (bSize*inode->i_block[t]+off));
			    if(directoryRead == -1) 
			      {
				fprintf(stderr, "Error: could not read directory\n");
				exit(1);
			      }
			    if(directoryEntry->inode > 0) 
			      {
				fprintf(stdout, "DIRENT,%u,%d,%u,%u,%u,'%s'\n",x+1, off, directoryEntry->inode, directoryEntry->rec_len, directoryEntry->name_len, directoryEntry->name); 
			      }
			    off += directoryEntry->rec_len;
			  }
		      }
		    t++;
		  }
	      }
	    __u32 indBlock = inode->i_block[EXT2_IND_BLOCK];
	    __u32 dindBlock = inode->i_block[EXT2_DIND_BLOCK];
	    __u32 tindBlock = inode->i_block[EXT2_TIND_BLOCK];	        
	    if(indBlock != 0)
	      {
		__u32 *indirectElement=malloc(bSize*sizeof(__u32));
		printingIndirectReferences(indBlock, 1, x+1, 12, indirectElement);
	      }
	    if(dindBlock != 0)
	      {
		__u32 *indirectElement=malloc(bSize*sizeof(__u32));
		printingIndirectReferences(dindBlock, 2, x+1, 268, indirectElement);
	      }
	    if(tindBlock != 0)
	      {
		__u32 *indirectElement=malloc(bSize*sizeof(__u32));
		printingIndirectReferences(tindBlock, 3, x+1, 65804, indirectElement);
	      }
	  }
	}
      i++;
    }
  free(inodeTable);
}

int main(int argc, char** argv) 
{
  numGroups = 0;
  setupArgs(argc, argv);
  superBlockInfo();
  groupInfo();
  freeInfo();
  inodeInfo();
  exit(0);
}
