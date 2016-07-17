#include "../include/ctb.h"
#include "../include/portscan.h"

#include <sstream>

#ifndef _WIN32
# include <glob.h>
#endif

namespace ctb {

    bool GetAvailablePorts( std::vector<std::string>& result,
					   bool checkInUse )
    {
#ifdef _WIN32

	   std::stringstream devname;

	   for( int i = 1; i < 100; i++ ) {

		  devname.clear(); devname.str( "" );

		  // some systems like WinCE doesn't like the extended port numbering...
		  i < 10 ? devname << "com" << i : devname << "\\\\.\\com" << i;

		  COMMCONFIG cc;

		  DWORD dwSize = sizeof( cc );

		  if ( ::GetDefaultCommConfig( devname.str().c_str(), &cc, &dwSize ) ) {

			 if( cc.dwProviderSubType == PST_RS232 ) {

				ctb::SerialPort com;

				if( com.Open( devname.str().c_str() ) < 0 ) {

				    continue;

				}

				result.push_back( devname.str().c_str() );

			 }
		  }
	   }

#else
	   glob_t globbuf;
	   int res;

	   // search for standard serial ports
	   /*res = glob( "/dev/ttyS*", GLOB_ERR, NULL, &globbuf );

	   if( res == 0 ) {

		  // no error, glob was successful
		  for( int i = 0; i < (int)globbuf.gl_pathc; i++ ) {

			 if( checkInUse ) {

				ctb::SerialPort com;

				if( com.Open( globbuf.gl_pathv[ i ] ) < 0 ) {

				    continue;

				}

				result.push_back( std::string( globbuf.gl_pathv[ i ] ) );

			 }
		  }

	   }
	   globfree( &globbuf );*/

	   // search for USB to RS232 converters
	   res = glob( "/dev/ttyUSB*", GLOB_ERR, NULL, &globbuf );

	   if( res == 0 ) {

		  // no error, glob was successful
		  for( int i = 0; i < (int)globbuf.gl_pathc; i++ ) {

			 if( checkInUse ) {

				ctb::SerialPort com;

				if( com.Open( globbuf.gl_pathv[ i ] ) < 0 ) {

				    continue;

				}

				result.push_back( std::string( globbuf.gl_pathv[ i ] ) );

			 }
		  }

	   }

	   globfree( &globbuf );

	   // search for ACM* ports
	   res = glob( "/dev/ttyACM*", GLOB_ERR, NULL, &globbuf );

	   if( res == 0 ) {

		  // no error, glob was successful
		  for( int i = 0; i < (int)globbuf.gl_pathc; i++ ) {

			 if( checkInUse ) {

				ctb::SerialPort com;

				if( com.Open( globbuf.gl_pathv[ i ] ) < 0 ) {

				    continue;

				}

				result.push_back( std::string( globbuf.gl_pathv[ i ] ) );

			 }
		  }

	   }

	   globfree( &globbuf );
#endif

    return result.size();

    }

} // namespace ctb
