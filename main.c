#include <dirent.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>

static uint32_t crc32_tab[] = {
    0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
    0xe963a535, 0x9e6495a3,	0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
    0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
    0xf3b97148, 0x84be41de,	0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
    0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec,	0x14015c4f, 0x63066cd9,
    0xfa0f3d63, 0x8d080df5,	0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
    0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,	0x35b5a8fa, 0x42b2986c,
    0xdbbbc9d6, 0xacbcf940,	0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
    0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
    0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
    0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,	0x76dc4190, 0x01db7106,
    0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
    0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
    0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
    0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
    0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
    0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
    0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
    0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
    0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
    0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
    0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
    0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
    0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
    0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
    0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
    0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
    0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
    0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
    0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
    0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
    0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
    0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
    0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
    0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
    0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
    0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
    0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
    0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
    0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
    0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
    0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
    0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};


int is_regular_file(const char *path)
{
    struct stat path_stat;
    if(stat(path, &path_stat) == 0)
        return S_ISREG(path_stat.st_mode);
    else
        return -1;
}
int file_size(const char *path)
{
    int size;
    struct stat st;
    if(is_regular_file(path) == 1)
    {
        stat(path, &st);
        size = st.st_size;
        return size;
    }
    return -1;
}

int list_dir( char *sdir, void (*cb)(char *file) )
{
    char          *sdirbuffer = 0;
    DIR           *d;
    struct dirent *dir;
    d = opendir(sdir);
    if(!sdirbuffer)
        sdirbuffer = strdup(sdir);
    if (sdirbuffer)
    if (d)
    {
      while ((dir = readdir(d)) != NULL)
      {
        char *str = malloc(strlen(sdirbuffer) + strlen(dir->d_name)+2);
        memset(str,0,strlen(sdirbuffer) + strlen(dir->d_name));
        strcat(str,sdirbuffer);
        strcat(str,"/");
        strcat(str,dir->d_name);
        if(is_regular_file(str))
        {
            cb(str);
        }
        else
        {
            if((strcmp(dir->d_name,".") == 0) || (strcmp(dir->d_name,"..") == 0))
            {
                free(str);
                continue;
            }
            list_dir(str,cb);
            free(str);
        }

      }
      if(sdirbuffer)
          free(sdirbuffer);
      closedir(d);
    }
    return 0;
}

static char *extbuffer;

static int extbuffer_size = 0;
static uint32_t head_size_of_list = 0;

static uint32_t ssize = 0;
static uint32_t offset = 0;
static uint32_t size = 0;

struct ffs_header_s {
    uint32_t list_size;
    uint32_t data_size;
    uint32_t crc_header;
    uint32_t crc_list;
    uint32_t crc_data;
};

struct ffs_list_item_s {
    uint32_t data_offset;
    uint32_t data_size;
    uint8_t file_name_len;
    uint8_t file_name[];
};
struct ffs_header_s head;
struct ffs_list_item_s list;
struct header_s {
    char path[_MAX_PATH/4];
    uint32_t size;
};

void callback(char *file)
{
    uint8_t i = 0, j = 0;
    FILE *fp;
    size = file_size(file);
    i = strlen(file);
    i = i-3;
    fp = fopen(file,"rb");
    static char *pbuffer = 0;
    if(pbuffer == 0)
        pbuffer = extbuffer+20;

    if(fp){
        struct ffs_list_item_s *header = (struct ffs_list_item_s*)pbuffer;
        //memset(header,0,sizeof(struct ffs_list_item_s));   // block of memory
        header->data_offset = offset;
        offset += size;
        header->data_size = size;
        header->file_name_len = i;
        strcat(header->file_name,file+4);
        printf("name: %s\n",header->file_name);
        pbuffer+=i+8;
        *(pbuffer) = 0x00;
        pbuffer++;
        extbuffer_size+=i+9;
        head_size_of_list += i+9;//sizeof(struct ffs_list_item_s)
        printf("size: %d\n",head_size_of_list);
        fclose(fp);

    }
}

void callback_list(char *file)
{
    static char rbuff[256];
    FILE *fp;
    int rsize = 0;
    size = file_size(file);
    fp = fopen(file,"rb");
    static char *pbuffer = 0;
    if(pbuffer == 0)
        pbuffer = extbuffer+20 + head_size_of_list;

    if(fp){
        int fsize = 0;
        //struct ffs_list_item_s *header = (struct ffs_list_item_s*)pbuffer;

        while(1)
         {
             rsize = fread(rbuff,1,256,fp);
             if(rsize == 0)
                 break;
             fsize+=rsize;
             memcpy(pbuffer,rbuff,rsize);
             pbuffer+=rsize;
             extbuffer_size+=rsize;
             if(rsize < 256)
                 break;
         }
         //header->data_size = fsize;
         fclose(fp);
   }

}
static uint32_t crc_of_list;

uint32_t crc()
{
    uint32_t crc32 =0;
    static char *pbuffer = 0;
    if(pbuffer == 0)
        pbuffer = extbuffer+20;

    while (pbuffer <= extbuffer + 20 + head_size_of_list)
    {
        crc32 = crc32_tab[(crc32^ (*pbuffer)) &0xFF]^(crc32>>8);
        pbuffer += 4;
    }
    crc_of_list = crc32;
    while(pbuffer <= extbuffer + 2*1024*1024)
    {
        crc32 = crc32_tab[(crc32^ (*pbuffer)) &0xFF]^(crc32>>8);
        pbuffer += 4;
    }
    return crc32;
}
uint32_t crc_of_file_system()
{
    uint32_t crc32 =0;
    static char *pbuffer = 0;
    if(pbuffer == 0)
        pbuffer = extbuffer+20 + head_size_of_list;
    while(pbuffer <= extbuffer + 2*1024*1024)
    {
        crc32 = crc32_tab[(crc32^ (*pbuffer)) &0xFF]^(crc32>>8);
        pbuffer += 4;
    }
    return crc32;
}

int main( int argc, char **argv)
{
  FILE *fp;
  fp = fopen ( "out.bin", "wb" );
  extbuffer = malloc(2*1024*1024*1);
  memset(extbuffer,0,2*1024*1024*1);
  memset(extbuffer, 255, 20);
  char *pbuffer = 0;
  if(pbuffer == 0)
          pbuffer = extbuffer;
  if(extbuffer)
  {
      list_dir("./fs",callback);
      struct ffs_header_s *header1 = (struct ffs_header_s*)pbuffer;
      printf("size: %d\n",head_size_of_list);
      header1->list_size = head_size_of_list+20-1;
      header1->data_size = offset;
      printf("size: %d\n",offset);
      offset = 0;
      //fclose(fp);
      //fp = fopen ( "out.bin", "w" );
      list_dir("./fs",callback_list);

      header1->crc_header = crc();
      header1->crc_list = crc_of_list;
      header1->crc_data = crc_of_file_system();
      fwrite(extbuffer,1,extbuffer_size+20,fp);
      printf("size: %d\n",header1->crc_header);
      fclose(fp);
      free(extbuffer);
      printf("size: %d\n",head_size_of_list);
      printf("size: %d\n",offset);
  }
  return 0;
}
