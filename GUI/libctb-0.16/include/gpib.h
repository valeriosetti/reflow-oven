#ifndef LIBCTB_GPIB_H_INCLUDED_
#define LIBCTB_GPIB_H_INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// Name:        gpibx.h
// Purpose:     base class for gpib devices
// Author:      Joachim Buermann
// Copyright:   (c) 2010 Joachim Buermann
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "iobase.h"

namespace ctb {

/*!
  \file gpib.h
*/

    /*! defines the os specific name for the first gpib controller */
    extern const char* GPIB1;

    /*! defines the os specific name for the second gpib controller */
    extern const char* GPIB2;

    /*!
	 \enum GpibTimeout
	 NI488.2 API defines the following valid timeouts.
    */
    enum GpibTimeout
    {
	   /// no timeout (infinity)
	   GpibTimeoutNone = 0,
	   /// 10 micro seconds
	   GpibTimeout10us,
	   /// 30 micro seconds
	   GpibTimeout30us,
	   /// 100 micro seconds
	   GpibTimeout100us,
	   /// 300 micro seconds
	   GpibTimeout300us,
	   /// 1 milli second
	   GpibTimeout1ms,
	   /// 3 milli seconds
	   GpibTimeout3ms,
	   /// 10 milli seconds
	   GpibTimeout10ms,
	   /// 30 milli seconds
	   GpibTimeout30ms,
	   /// 0.1 seconds
	   GpibTimeout100ms,
	   /// 0.3 seconds
	   GpibTimeout300ms,
	   /// 1 second
	   GpibTimeout1s,
	   /// 3 seconds
	   GpibTimeout3s,
	   /// 10 seconds
	   GpibTimeout10s,
	   /// 30 seconds
	   GpibTimeout30s,
	   /// 100 seconds
	   GpibTimeout100s,
	   /// 300 seconds (5 minutes)
	   GpibTimeout300s,
	   /// 1000 seconds
	   GpibTimeout1000s
    };

    /*!
	 \struct Gpib_DCS

	 The device control struct for the gpib communication class.
	 This struct should be used, if you refer advanced parameter.
    */
    struct Gpib_DCS
    {
	   /*! primary address of GPIB device */
	   int m_address1;
	   /*! secondary address of GPIB device */
	   int m_address2;
	   /*! I/O timeout */
	   GpibTimeout m_timeout;
	   /*! EOT enable */
	   bool m_eot;
	   /*!
		Defines the EOS character.
		Note! Defining an EOS byte does not cause the driver to
		automatically send that byte at the end of write I/O
		operations. The application is responsible for placing the
		EOS byte at the end of the data strings that it defines.
		(National Instruments NI-488.2M Function Reference Manual)
	   */
	   unsigned char m_eosChar;
	   /*!
		Set the EOS mode (handling).m_eosMode may be a combination
		of bits ORed together. The following bits can be used:
		0x04: Terminate read when EOS is detected.
		0x08: Set EOI (End or identify line) with EOS on write function
		0x10: Compare all 8 bits of EOS byte rather than low 7 bits
		(all read and write functions).
	   */
	   unsigned char m_eosMode;
	   /*! buffer for internal use */
	   char m_buf[32];
	   /*! to avoid memory leak warnings generated by swig */
	   ~Gpib_DCS() {};
	   /*!
		\brief the constructor initiate the device control struct with
		the common useful values and set the internal timeout for the
		GPIB controller to 1ms to avoid (or better reduce) blocking
	   */
	   Gpib_DCS() {
		  /*! set default device address to 1 */
		  m_address1 = 1;
		  m_address2 = 0;
		  /*!
		    set the timeout to a short value to avoid blocking
		    (default are 1msec)
		  */
		  m_timeout = GpibTimeout1ms;
		  m_eot = true;
		  /*! EOS character, see above! */
		  m_eosChar = 0;//'\n';
		  /*! EOS mode, see above! */
		  m_eosMode = 0;
	   };
	   /*!
		\brief returns the internal parameters in a more human readable
		string format like 'Adr: (1,0) to:1ms'.
		\return the settings as a null terminated string
	   */
	   char* GetSettings();
    };

/*!
  \enum GpibIoctls

  The following Ioctl calls are only valid for the GpibDevice class.
*/
    enum GpibIoctls {
	   /*!
		Set the adress of the via gpib connected device.
	   */
	   CTB_GPIB_SETADR = CTB_GPIB,
	   /*!
		Get the serial poll byte
	   */
	   CTB_GPIB_GETRSP,
	   /*!
		Get the GPIB status
	   */
	   CTB_GPIB_GETSTA,
	   /*!
		Get the last GPIB error number
	   */
	   CTB_GPIB_GETERR,
	   /*!
		Get the GPIB line status (hardware control lines) as an
		integer. The lowest 8 bits correspond to the current state
		of the lines.
	   */
	   CTB_GPIB_GETLINES,
	   /*!
		Set the GPIB specific timeout
	   */
	   CTB_GPIB_SETTIMEOUT,
	   /*!
		Forces the specified device to go to local program mode
	   */
	   CTB_GPIB_GTL,
	   /*!
		This routine can only be used if the specified GPIB
		Interface Board is the System Controller.
		Remember that even though the REN line is asserted,
		the device(s) will not be put into remote state until is
		addressed to listen by the Active Controller
	   */
	   CTB_GPIB_REN,
	   /*!
		The command asserts the GPIB interface clear (IFC) line for
		ast least 100us if the GPIB board is the system controller.
		This initializes the GPIB and makes the interface CIC and
		active controller with ATN asserted.
		Note! The IFC signal resets only the GPIB interface functions
		of the bus devices and not the internal device functions.
		For a device reset you should use the CTB_RESET command above.
	   */
	   CTB_GPIB_RESET_BUS,
	   /*!
		Configure the end-of-string (EOS) termination character.
		Note! Defining an EOS byte does not cause the driver to
		automatically send that byte at the end of write I/O
		operations. The application is responsible for placing the
		EOS byte at the end of the data strings that it defines.
		(National Instruments NI-488.2M Function Reference Manual)
	   */
	   CTB_GPIB_SET_EOS_CHAR,
	   /*!
		Get the internal EOS termination character (see above).
	   */
	   CTB_GPIB_GET_EOS_CHAR,
	   /*!
		Set the EOS mode (handling).m_eosMode may be a combination
		of bits ORed together. The following bits can be used:
		0x04: Terminate read when EOS is detected.
		0x08: Set EOI (End or identify line) with EOS on write function
		0x10: Compare all 8 bits of EOS byte rather than low 7 bits
		(all read and write functions).
	   */
	   CTB_GPIB_SET_EOS_MODE,
	   /*!
		Get the internal EOS mode (see above).
	   */
	   CTB_GPIB_GET_EOS_MODE,
    };

/*!
  \class GpibDevice
  GpibDevice is the basic class for communication via the GPIB bus.
*/
    class GpibDevice : public IOBase
    {
    protected:
	   /*!
		\brief
		the internal board identifier, 0 for the first gpib controller,
		1 for the second one
	   */
	   int m_board;
	   /*!
		\brief
		the file descriptor of the connected gpib device
	   */
	   int m_hd;
	   /*!
		\brief
		contains the internal conditions of the GPIB communication like
		GPIB error, timeout and so on...
	   */
	   int m_state;
	   /*! the internal GPIB error number */
	   int m_error;
	   /*! the count of data read or written */
	   int m_count;
	   /*!
		\brief contains the internal settings of the GPIB connection like
		address, timeout, end of string character and so one...
	   */
	   Gpib_DCS m_dcs;
	   int CloseDevice();
	   /*!
		\brief returns a short notation or more detail description of
		the given GPIB error number.
		\param error the occured GPIB error
		\param detailed true for a more detailed description, false
		otherwise
		\return a null terminated string with the short or detailed
		error message.
	   */
	   virtual const char* GetErrorString(int error,bool detailed);
	   /*!
		Open the interface (internally to request a file descriptor for the
		given interface). The second parameter is a undefined pointer of a
		Gpib_DCS data struct.
		\param devname the name of the GPIB device, GPIB1 means the first
		GPIB controller, GPIB2 the second (if available).
		\param dcs untyped pointer of advanced device parameters,
		\sa struct Gpib_DCS (data struct for the gpib device)
		\return zero on success, otherwise -1
	   */
	   int OpenDevice(const char* devname, void* dcs);
    public:
	   GpibDevice() {
		  m_board = -1;
		  m_hd = -1;
		  m_state = m_count = m_error = 0;
	   };
	   virtual ~GpibDevice() {Close();};
	   /*!
		\brief returns the name of the class instance. You find this useful,
		if you handle different devices like a serial port or a gpib device
		via a IOBase pointer.
		\return name of the class.
	   */
	   const char* ClassName() {return "ctb::GpibDevice";};
	   /*!
		\brief returns a more detail description of the given error
		number.
		\param error the occured error number
		\return null terminated string with the error description
	   */
	   virtual const char* GetErrorDescription(int error) {
		  return GetErrorString(error,true);
	   };
	   /*!
		\brief returns a short notation like 'EABO' of the given error
		number.
		\param error the occured error number
		\return null terminated string with the short error notation
	   */
	   virtual const char* GetErrorNotation(int error) {
		  return GetErrorString(error,false);
	   };
	   /*!
		\brief request the current settings of the connected gpib device
		as a null terminated string.
		\return the settings as a string like 'Adr: (1,0) to:1ms'
	   */
	   virtual char* GetSettingsAsString() {
		  return m_dcs.GetSettings();
	   };
	   /// This is only for internal usage
	   int Ibrd(char* buf,size_t len);
	   /// This is only for internal usage
	   int Ibwrt(char* buf,size_t len);
	   /*!
		\brief Many operating characteristics are only possible for
		special devices. To avoid the need of a lot of different functions
		and to give the user a uniform interface, all this special
		operating instructions will covered by one Ioctl methode (like
		the linux ioctl call).
		The Ioctl command (cmd) has encoded in it whether the argument
		is an in parameter or out parameter, and the size of the
		argument args in bytes. Macros and defines used in specifying an
		ioctl request are located in iobase.h and the header file for
		the derivated device (for example in gpib.h).
		\param cmd one of GpibIoctls specify the ioctl request.
		\param args is a typeless pointer to a memory location, where
		Ioctl reads the request arguments or write the results.
		Please note, that an invalid memory location or size involving
		a buffer overflow or segmention fault!
	   */
	   virtual int Ioctl(int cmd,void* args);
	   /*!
		Returns the current state of the device.
		\return 1 if device is valid and open, otherwise 0
	   */
	   int IsOpen() {
		  return m_hd >= 0;
	   };

	   /*!
	     \brief Opens a GPIB device in a user likely way. Insteed of
	     using the Device Control Struct just input your parameter in a
	     more intuitive manner.
	     \param devname the name of the GPIB controler like GPIB1 or GPIB2
	     \param address the address of the connected device (1...31)
	     \return the new file descriptor, or -1 if an error occurred
	    */
	   int Open( const char* devname, int address );

	   // Open overrides and hides the IOBase::Open( const char*
	   // devname, void* dcs ) of the base class! So bring it into scope
	   // again!
	   using IOBase::Open;

	   int Read(char* buf,size_t len);

	   int Write(char* buf,size_t len);

	   /*!
		\brief FindListener returns all listening devices connected to
		the GPIB bus of the given board.
		This function is not member of the GPIB class, becauce it
		should do it's job before you open any GPIB connection.
		\param board the board nummber. Default is the first board (=0).
		Valid board numbers are 0 and 1.
		\return -1 if an error occurred, otherwise a setting bit for
		each listener address. Bit0 is always 0 (address 0 isn't valid,
		Bit1 means address 1, Bit2 address 2 and so on...
	   */
	   static int FindListeners(int board = 0);
    };

} // namespace ctb

#endif