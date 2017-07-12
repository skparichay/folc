#include <iostream>
#include <stdio.h>
#include <vector>
#include <map>
#include <iterator>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

#define PCT_SIZE 3084800
# define IMG_DIFF 12800
# define IMG_Size 3072000
# define STREAM_HEADER 2560

class VDRtoYUV {

  private:

  uint64_t m_FileSize;

  unsigned int m_VideoWidth;
  unsigned int m_VideoHeight;
  unsigned int m_VDR_Frm_Count;
  FILE * m_pFile1;
  FILE * m_pFile2;
  FILE * m_pFile3;
  std::vector < unsigned char > m_Buf1, m_Buf2;

  vector < unsigned char > m_Frm_Data;

  public:

    uint64_t m_TotalPackages;

  VDRtoYUV() {
    m_pFile1 = 0;
    m_pFile2 = 0;
    m_pFile3 = 0;
    m_FileSize = 0;
    m_VDR_Frm_Count = 0;
    m_Buf1.reserve(IMG_Size / 2);
    m_Buf2.reserve(IMG_Size / 2);
    m_Frm_Data.reserve(IMG_Size);

  }

  ~VDRtoYUV() {
    m_Buf1.clear();
    m_Buf2.clear();
    m_Frm_Data.clear();

    if (m_pFile1) {
      fclose(m_pFile1);
    }
    if (m_pFile2) {
      fclose(m_pFile2);
    }
    if (m_pFile3) {
      fclose(m_pFile3);
    }

  }

    int FileopenCheck(const char *File,  const char * OpenMode) {

      FILE * fp = fopen(File, OpenMode);

      if (fp == NULL) {
        cout << " Err : " << string(File) << ": File Opening issue \n" << endl;
        return 1;
      }

      return 0;
    }

int GetTimestamp()
{
rewind(m_pFile1);

fseeko(m_pFile1, 2560, SEEK_SET);


 vector<uint64_t> timestamp;
    uint64_t temp;

      for (int i = 0;  i < m_TotalPackages; i++) {

	fread(&temp, sizeof(uint64_t), 1, m_pFile1);
	timestamp.push_back(temp);

      fseeko(m_pFile1, PCT_SIZE-8, SEEK_CUR);

   } 

 std::ofstream outFile;
    outFile.open("VdrTimestamp.txt");
    for (int i =0; i< m_TotalPackages;i++) outFile << timestamp[i] << "\n";

   timestamp.clear();  
std::cout << "m_TotalPackets: " << m_TotalPackages  <<std::endl;

rewind(m_pFile1);


return 0;
}
 int FrameCntandValidation(const char *InputFile,const char *OutFile1,const char *OutFile2)
 {
      m_pFile1 = fopen(InputFile, "rb+");
      fseeko( m_pFile1, 0, SEEK_END);
      m_FileSize = ftello(m_pFile1);

      if (!m_FileSize) 
      {
        cout << "Error : Input File size Zero \n" << endl;
        return 1;
      }

      if (m_FileSize < STREAM_HEADER) {
        cout << "Error : Input File  does not contain any header and vdr frame \n" << endl;
        return 1;
      }

      uint64_t filesize = m_FileSize - STREAM_HEADER;

      if (filesize < PCT_SIZE) {
        cout << "Error : Input File  does not contain vaild vdr frame \n" << endl;
        return 1;
      }

     m_TotalPackages = filesize / PCT_SIZE;

      fseeko(m_pFile1, 8, SEEK_SET);

      fread(&m_VideoWidth, sizeof(unsigned int), 1, m_pFile1);
      fread(&m_VideoHeight, sizeof(unsigned int), 1, m_pFile1);

      fclose(m_pFile1);
      m_pFile1 = 0;


      m_pFile1 = fopen(InputFile, "rb+"); //open the vdr file in read mode
      m_pFile2 = fopen(OutFile1, "wb+"); //open the yuv file in write mode
      m_pFile3 = fopen(OutFile2, "wb+"); //open the yuv file in write mode


      return 0;
    }
  int FrameData_ReadWrite()
{


      fseeko(m_pFile1, STREAM_HEADER , SEEK_SET);	  

#if 1    // For All frames

for (unsigned int k= 0; k < m_TotalPackages ; k++ ) {

      fseeko(m_pFile1,  IMG_DIFF, SEEK_CUR);
      fread(m_Frm_Data.data(), IMG_Size, 1, m_pFile1);

      unsigned char * temp1 = m_Buf1.data();
      unsigned char * temp2 = m_Buf2.data();

      //extract yuv format frame data from .vdr file
      unsigned int j = 0;
      for (unsigned int i = 0; i < m_VideoHeight; i++, j += m_VideoWidth) {

        memcpy(temp1, m_Frm_Data.data() + j, m_VideoWidth / 2);
        temp1 += m_VideoWidth / 2;

        memcpy(temp2, m_Frm_Data.data() + j + m_VideoWidth / 2, m_VideoWidth / 2);
        temp2 += m_VideoWidth / 2;
      }
      Mat Front(m_VideoHeight, m_VideoWidth/2, CV_8UC1, (unsigned char *) m_Buf1.data());
      Mat Rear(m_VideoHeight, m_VideoWidth/2, CV_8UC1, (unsigned char *) m_Buf2.data());
      imshow("Front",Front);
      imshow("Rear",Rear);

//      fwrite(m_Buf1.data(), IMG_Size / 2, 1, m_pFile2);
//      fwrite(m_Buf2.data(), IMG_Size / 2, 1, m_pFile3);

}

#else  // Single frame
      int Framenum = 0; 
  
      fseeko(m_pFile1, (Framenum)*PCT_SIZE , SEEK_CUR);
      fseeko(m_pFile1,  IMG_DIFF, SEEK_CUR);
      fread(m_Frm_Data.data(), IMG_Size, 1, m_pFile1);

      unsigned char * temp1 = m_Buf1.data();
      unsigned char * temp2 = m_Buf2.data();

      //extract yuv format frame data from .vdr file
      unsigned int j = 0;
      int count= 0;
      for (unsigned int i = 0; i < ((m_VideoHeight * 3) / 2); i++, j += m_VideoWidth) {

        memcpy(temp1, m_Frm_Data.data() + j, m_VideoWidth / 2);
        temp1 += m_VideoWidth / 2;

        memcpy(temp2, m_Frm_Data.data() + j + m_VideoWidth / 2, m_VideoWidth / 2);
        temp2 += m_VideoWidth / 2;
        count++;
      }
      cout << count << endl << flush;
//      Mat frameData(m_Buf1, true);

      /////////////////////////////////////////////////////////////////////////////


#endif 

	return 0;
}

};
