#include "./VdrToYuvExtraction.h"


int main(int argc, char *argv[]) {


int min = 0;

    unsigned int TotalPackages = 0;

    if (argc != 4) {
      std::cout << "///////////////////////////////////////////////////////////////////////////\n";
      std::cout << "//  Usage : exe  InputVDR   YUV1   YUV2                  //\n";
      std::cout << "///////////////////////////////////////////////////////////////////////////\n\n";

      return 1;
    }

    std::cout << "//////////////////////////////////////////////////////////////////\n";
    std::cout << "//       Conversion of .vdr file to YUVs Files Extraction         //\n";
    std::cout << "//////////////////////////////////////////////////////////////////\n\n";

    cout << "Input VDR File:" << argv[1] << endl;
    cout << "Output YUV File1:" << argv[2] << endl;
    cout << "Output YUV File2:" << argv[3] << endl;


{

    VDRtoYUV Vdr1;


    if (Vdr1.FileopenCheck(argv[1], "rb+"))
      return 1;

    if (Vdr1.FileopenCheck(argv[2], "wb+"))
      return 1;

    if (Vdr1.FileopenCheck(argv[3], "wb+"))
      return 1;

    ///////////////////////////////////////////////////////////////////////////////////////////////

    if (Vdr1.FrameCntandValidation(argv[1],argv[2],argv[3])) {
	return 1;
     }

    Vdr1.GetTimestamp();

        if (Vdr1.FrameData_ReadWrite()) {
          return 1;
}

   
} std::cout << "\n\n//-------------------  Conversion completed and YUV files generated ---------------- //\n";

}

