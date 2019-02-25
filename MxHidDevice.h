/*
 * File:	MxHidDevice.h
 *
 * Copyright (c) 2010 Freescale Semiconductor, Inc. All rights reserved.
 * See included license file for license details.
*/

#ifndef   __MXHIDDEVICE_H__
#define   __MXHIDDEVICE_H__
#pragma once

// Code from AtkHostApiClass.h
// Could be moved to SDP protocol or MxRomApi or something like that.
#define ROM_KERNEL_CMD_COMPLETE 0x0
#define ROM_KERNEL_CMD_SIZE 0x10
//#define ROM_KERNEL_WF_FT_CSF 0xCC
//#define ROM_KERNEL_WF_FT_DCD 0xEE
//#define ROM_KERNEL_WF_FT_APP 0xAA
//#define ROM_KERNEL_WF_FT_OTH 0x0
#define MAX_SIZE_PER_FLASH_COMMAND 0x200000
#define MAX_SIZE_PER_DOWNLOAD_COMMAND 0x200000

/* Command Packet Format: Header(2)+Address(4)+Format(1)+ByteCount(4)+Data(4) */
#define ROM_KERNEL_CMD_RD_MEM 0x0101
#define ROM_KERNEL_CMD_WR_MEM 0x0202
#define ROM_KERNEL_CMD_WR_FILE 0x0404
#define ROM_KERNEL_CMD_ERROR_STATUS 0x0505
#define RAM_KERNEL_CMD_HEADER 0x0606
//#define ROM_KERNEL_CMD_RE_ENUM 0x0909
#define ROM_KERNEL_CMD_DCD_WRITE 0x0A0A
#define ROM_KERNEL_CMD_JUMP_ADDR 0x0B0B
#define ROM_KERNEL_CMD_SKIP_DCD_HEADER  0x0C0C


#define MAX_DCD_WRITE_REG_CNT    85
#define ROM_WRITE_ACK   0x128A8A12
#define ROM_STATUS_ACK  0x88888888
#define ROM_OK_ACK		0x900DD009

#define ROM_STATUS_ACK2 0xF0F0F0F0

#define SDP_CMD_SIZE    16      
#define SDP_REPORT_LENGTH (SDP_CMD_SIZE+1)

#define REPORT_ID_SDP_CMD  1
#define REPORT_ID_DATA     2
#define REPORT_ID_HAB_MODE 3
#define REPORT_ID_STATUS   4

#define RAM_KERNEL_ACK_SIZE 0x08
#define RAM_KERNEL_CMD_SIZE 0x10
#define WR_BUF_MAX_SIZE 0x4000

#define MAX_MODEL_LEN	128
#define MINUM_TRANSFER_SIZE 0x20

#define IVT_BARKER_HEADER      0x402000D1
#define IVT_BARKER2_HEADER      0x412000D1
#define MX8_IVT_BARKER_HEADER	0x434000D1
#define MX8_IVT2_BARKER_HEADER  0x433000de

#define ROM_TRANSFER_SIZE	   0x400

#define FLASH_HEADER_SIZE	   0x20
#define ROM_TRANSFER_SIZE	   0x400
#define DCD_BARKER_CODE        0xB17219E9
#define IRAM_START_ADDRESS     0xF8000000
#define IVT_OFFSET             0x400
#define HAB_CMD_WRT_DAT        0xcc  /**< Write Data */
#define HAB_CMD_CHK_DAT        0xcf  /**< Check Data */
#define HAB_TAG_DCD            0xd2       /**< Device Configuration Data */
#define HAB_DCD_BYTES_MAX     1768 

// Multi-image suppport
#define MX8_MAX_IMAGES_COUNT			4
// 8KB Initial Image size = IVT1 + IVT2 + DCD + CSF + BootDatas+ Img info
#define MX8_INITIAL_IMAGE_SIZE			0x1000
#define MX8_IMG_OFFSET				0x8000

#define IVT_OFFSET_SD	0x400

#define ROM_ECC_SIZE_ALIGN 0x400

#define IH_MAGIC	0x27051956	/* Image Magic Number		*/
#define IH_NMLEN		32	/* Image Name Length		*/

// Address ranges for Production parts: 

typedef struct image_header {
	uint32_t	ih_magic;	/* Image Header Magic Number	*/
	uint32_t	ih_hcrc;	/* Image Header CRC Checksum	*/
	uint32_t	ih_time;	/* Image Creation Timestamp	*/
	uint32_t	ih_size;	/* Image Data Size		*/
	uint32_t	ih_load;	/* Data	 Load  Address		*/
	uint32_t	ih_ep;		/* Entry Point Address		*/
	uint32_t	ih_dcrc;	/* Image Data CRC Checksum	*/
	uint8_t		ih_os;		/* Operating System		*/
	uint8_t		ih_arch;	/* CPU architecture		*/
	uint8_t		ih_type;	/* Image Type			*/
	uint8_t		ih_comp;	/* Compression Type		*/
	uint8_t		ih_name[IH_NMLEN];	/* Image Name		*/
} image_header_t;


/// <summary>
/// A MxHidDevice device.
/// </summary>
class MxHidDevice :public CHidDevice
{

public:
	struct Response
	{
		unsigned int romResponse;
		struct
		{
			unsigned short ack;		// ack
			unsigned short csum;	// data checksum
			unsigned long len;		// data len
		};
	};

	enum MemorySection { MemSectionOTH = 0x00, MemSectionAPP = 0xAA, MemSectionCSF = 0xCC, MemSectionDCD = 0xEE };
	static MemorySection StringToMemorySection(CString section)
	{
		if (section.CompareNoCase(_T("DCD")) == 0)
			return MemSectionDCD;
		else if (section.CompareNoCase(_T("CSF")) == 0)
			return MemSectionCSF;
		else if (section.CompareNoCase(_T("APP")) == 0)
			return MemSectionAPP;
		else return MemSectionOTH;
	}

	enum MemoryAction { MemAction_None, MemAction_Set, MemAction_Jump };
	static MemoryAction StringToMemoryAction(CString action)
	{
		if (action.CompareNoCase(_T("Set")) == 0)
			return MemAction_Set;
		else if (action.CompareNoCase(_T("Jump")) == 0)
			return MemAction_Jump;
		else return MemAction_None;
	}

	enum MemoryType { LPDDR2_V3, LPDDR2, MDDR };

	typedef struct _MxTrans
	{
		//BOOL HasFlashHeader;
		UINT PhyRAMAddr4KRL;
		UINT ExecutingAddr;
		UINT CodeOffset;
	}ImageParameter, *PImageParameter;

	typedef struct _BootData
	{
		unsigned long ImageStartAddr;
		unsigned long ImageSize;
		unsigned long PluginFlag;
	}BootData, *PBootData;

	typedef struct _SubImageInfo
	{
		unsigned long long	Offset;
		unsigned long long	ImageAddr;
		unsigned long long	ImageEntry;
		unsigned long		ImageSize;
		unsigned long		ImageFlag;
		unsigned long		flag1;
		unsigned long		flag2;
	}SubImageInfo;

	typedef struct _BootDataV2
	{
		unsigned long	NrImages; // Number of images
		unsigned long	BootDataSize;
		unsigned long   BootDataFlag;
		unsigned long	Reserved;
		SubImageInfo	Images[MX8_MAX_IMAGES_COUNT];
		SubImageInfo	SCD;
		SubImageInfo	CSF;
		SubImageInfo	ImgReserved;
	}BootDataV2, *PBootDataV2;

	//DCD binary data format:
	//4 bytes for format	4 bytes for register1 address	4 bytes for register1 value to set
	//4 bytes for format	4 bytes for register2 address	4 bytes for register2 value to set
	//...
	typedef struct _RomFormatDCDData
	{
		UINT format;
		UINT addr;
		UINT data;
	}RomFormatDCDData, *PRomFormatDCDData;

	typedef struct _ImgFormatDCDData
	{
		unsigned long Address;
		unsigned long Data;
	}ImgFormatDCDData, *PImgFormatDCDData;

	enum eTask { INIT = 1, TRANS, EXEC, JUMP, RUN, RUN_PLUGIN, READ, WRITE};
	
	typedef struct _IvtHeader
	{
		unsigned long IvtBarker;
		unsigned long ImageStartAddr;// LONG(0x70004020)
		unsigned long Reserved;
		unsigned long DCDAddress;
		unsigned long BootData;
		unsigned long SelfAddr;// LONG(0x70004000)
		unsigned long Reserved2[2];
	}IvtHeader, *PIvtHeader;

	typedef struct _IvtHeaderV2
	{
		unsigned long		IvtBarker;
		unsigned long		Reserved;
		unsigned long long	DCDAddress;
		unsigned long long	BootData;
		unsigned long long	SelfAddr;
		unsigned long long	CSFAddr;
		union {
			unsigned long long	SCDAddr;
			unsigned long long	Next;
		};
		unsigned long long	Reserved2[2];
	}IvtHeaderV2, *PIvtHeaderV2;

	typedef struct _MxFunc
	{
		eTask Task;
		MemoryType   MemType;
		ImageParameter ImageParameter;
		PIvtHeader pIVT;
		unsigned int Addr;
		unsigned int Value;
		unsigned int offset;
	}MxFunc, *PMxFunc;

	
	typedef struct _FlashHeader
	{
		unsigned long ImageStartAddr;
		unsigned long Reserved[4];
	}FlashHeader, *PFlashHeader;

	MxHidDevice();
	virtual ~MxHidDevice(void);
	//struct Response SendRklCommand(unsigned short cmdId, unsigned long addr, unsigned long param1, unsigned long param2);
	//int GetRKLVersion(CString& fmodel, int& len, int& mxType);
	BOOL InitMemoryDevice(MemoryType MemType);
	//BOOL ProgramFlash(std::ifstream& file, UINT address, UINT cmdID, UINT flags, Device::UI_Callback callback);
	BOOL LoadFirmware(UCHAR *pBuffer, ULONGLONG dataCount, PMxFunc pMxFunc)
	{
		return m_IsFitImage ? LoadFitImage(pBuffer + pMxFunc->ImageParameter.CodeOffset, dataCount - pMxFunc->ImageParameter.CodeOffset, pMxFunc) 
							: Download(pBuffer, dataCount, pMxFunc->ImageParameter.PhyRAMAddr4KRL);
	}
	BOOL Download(UCHAR* pBuffer, ULONGLONG dataCount, UINT RAMAddress);
	BOOL Execute(UINT32 ImageStartAddr);
	BOOL Jump(UINT RAMAddress);
	BOOL RegRead(UINT address, UINT *value);
	BOOL RegWrite(UINT address, UINT value);
	BOOL SkipDCD();
	DWORD GetIvtOffset(DWORD *start, ULONGLONG dataCount, DWORD begin=0);
	BOOL RunMxMultiImg(UCHAR* pBuffer, ULONGLONG dataCount);
	BOOL MxHidDevice::RunPlugIn(UCHAR* pBuffer, ULONGLONG dataCount, PMxFunc pMxFunc);
	BOOL LoadFitImage(UCHAR *fit, ULONGLONG dataCount, PMxFunc pMxFunc);

	unsigned long long SCUViewAddr(unsigned long long addr)
	{
		if (addr >= 0x30000000 && addr < 0x40000000)
		{
			if (this->GetDevType() == MX8QM)
				return addr - 0x11000000;
		}

		return addr;
	}
	//BOOL Reset();

	// PROPERTIES
	//class MaxPacketSize : public Int32Property { public: int32_t get(); } _MaxPacketSize;
	typedef struct _MxRomParamt
	{
		CString cMXType;
		CString cSecurity;
		CString cRAMType;
		CString cMemInitFilePath;
	}MxRomParamt, *PMxRomParamt;
	MxRomParamt m_MxRomParamt;

private:
	enum HAB_t
	{
		HabUnknown = -1,
		HabEnabled = 0x12343412,
		HabDisabled = 0x56787856
	};

	enum ChipFamily_t
	{
		ChipUnknown = 0,
		MX508
	};

	typedef struct _SDPCmd
	{
		short command;
		char format;
		UINT address;
		UINT dataCount;
		UINT data;
	}SDPCmd, *PSDPCmd;

	typedef struct _DCD_Item
	{
		UINT type;
		UINT addr;
		UINT value;
	}DCD_Item, *PDCD_Item;
	UINT m_jumpAddr;

	BOOL MxHidDevice::DCDWrite(PUCHAR DataBuf, UINT RegCount);
	BOOL MxHidDevice::RunDCD(DWORD *pDCDRegion);
	ChipFamily_t GetChipFamily();
	BOOL MxHidDevice::GetCmdAck(UINT RequiredCmdAck);
	BOOL WriteMemory(UINT address, UINT data, UINT format);
	BOOL ValidAddress(const UINT address, const UINT format) const;
	//HAB_t GetHABType(ChipFamily_t chipType);
	//void PackRklCommand(unsigned char *cmd, unsigned short cmdId, unsigned long addr, unsigned long param1, unsigned long param2);
	//struct Response UnPackRklResponse(unsigned char *resBuf);
	BOOL AddIvtHdr(UINT32 ImageStartAddr);
	BOOL TransData(UINT address, UINT byteCount, const unsigned char * pBuf);
	BOOL ReadData(UINT address, UINT byteCount, unsigned char * pBuf);
	BOOL WriteToDevice(const unsigned char *buf, UINT count);
	BOOL ReadFromDevice(PUCHAR buf, UINT count);
	BOOL DeviceIoControl(DWORD controlCode, PVOID pRequest = NULL);
	BOOL OpenUSBHandle(HANDLE *pHandle, CString pipePath);
	VOID PackSDPCmd(PSDPCmd pSDPCmd);
	BOOL WriteReg(PSDPCmd pSDPCmd);
	BOOL MxHidDevice::SendCmd(PSDPCmd pSDPCmd);
	BOOL MxHidDevice::SendData(const unsigned char * DataBuf, UINT ByteCnt);
	BOOL MxHidDevice::GetHABType();
	BOOL MxHidDevice::GetDevAck(UINT RequiredCmdAck);
	ChipFamily_t _chipFamily;
	HAB_t _habType;
	//unsigned char _pSDPCmdBuf[SDP_REPORT_LENGTH];
	enum ChannelType { ChannelType_UART = 0, ChannelType_USB };
	BOOL m_IsFitImage;
};
#endif //  __MXHIDDEVICE_H__