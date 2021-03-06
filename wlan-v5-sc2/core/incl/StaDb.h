/*
*                Copyright 2002-2005, Marvell Semiconductor, Inc.
* This code contains confidential information of Marvell Semiconductor, Inc.
* No rights are granted herein under any patent, mask work right or copyright
* of Marvell or any third party.
* Marvell reserves the right at its sole discretion to request that this code
* be immediately returned to Marvell. This code is provided "as is".
* Marvell makes no warranties, express, implied or otherwise, regarding its
* accuracy, completeness or performance.
*/

/******************** (c) Marvell Semiconductor, Inc., 2001 *******************
*
* Purpose:
*    This file contains data types and routines used in accessing the
*    external station information database. This database contains
*    information on other stations (or APs) in a network. The information
*    includes the entity type (AP or station), the MAC address, the power
*    mode, and the station of the entity with respect to the station
*    containing this table.
*
* Public Procedures:
*    extStaDb_AddSta      Add a station to the table
*    extStaDb_DelSta      Delete a station from the table
*    extStaDb_SetState    Set the state of an external station
*    extStaDb_GetState    Get the state of an external station
*    extStaDb_SetPwrMode  Set the power mode of an external station
*    extStaDb_GetPwrMode  Get the power mode of an external station
*
* Notes:
*    None.
*
*****************************************************************************/

#ifndef _STADB_H_
#define _STADB_H_

/*============================================================================= */
/*                               INCLUDE FILES */
/*============================================================================= */
#include "wltypes.h"
#include "IEEE_types.h"
#include "osif.h"



#include "mib.h"
#include "smeMain.h"
#include "util.h"
#include "mib.h"
#include "buildModes.h"
#include "qos.h"
#include "mlme.h"
#include "ds.h"
#include "keyMgmtCommon.h"
#include "keyMgmt.h"
#include "List.h"
#include "ap8xLnxStaStats.h"
#include "ap8xLnxIntf.h" //add by lirui for traffic limit

/*add by zhanxuechao*/
#define IEEE80211_NODE_AUTH     0x00000001          /* authorized for data */
#define IEEE80211_NODE_QOS      0x00000002          /* QoS enabled */
#define IEEE80211_NODE_ERP      0x00000004          /* ERP enabled */
#define IEEE80211_NODE_HT       0x00000008          /* HT enabled */

/* add by zhanxuechao for authorize state */
#define IEEE80211_NODE_AUTHORIZE_SUCC       1
#define IEEE80211_NODE_AUTHORIZE_FAIL       0

/* NB: this must have the same value as IEEE80211_FC1_PWR_MGT */
#define IEEE80211_NODE_PWR_MGT  0x00000010          /* power save mode enabled */
#define IEEE80211_NODE_UAPSD    0x00000040          /* U-APSD power save enabled */
#define IEEE80211_NODE_UAPSD_TRIG 0x00000080        /* U-APSD triggerable state */
#define IEEE80211_NODE_UAPSD_SP 0x00000100          /* U-APSD SP in progress */
#define IEEE80211_NODE_ATH      0x00000200          /* Atheros Owl or follow-on device */
#define IEEE80211_NODE_OWL_WDSWAR 0x00000400        /* Owl WDS workaround needed*/
#define IEEE80211_NODE_WDS      0x00000800          /* WDS link */
#define IEEE80211_NODE_NOAMPDU  0x00001000          /* No AMPDU support */
#define IEEE80211_NODE_WEPTKIPAGGR 0x00002000       /* Atheros proprietary wep/tkip aggregation support */
#define IEEE80211_NODE_WEPTKIP  0x00004000
#define IEEE80211_NODE_TEMP     0x00008000          /* temp node (not in the node table) */
#define IEEE80211_NODE_40_INTOLERANT    0x00020000  /* HT40 Intolerant  */
#define IEEE80211_NODE_PAUSED   0x00040000          /* node is  paused*/
#define IEEE80211_NODE_EXTRADELIMWAR 0x00080000
#define IEEE80211_NODE_NAWDS 0x00100000          /* node is an NAWDS repeater */
#define IEEE80211_NODE_TDLS     0x00200000          /* TDLS node */
#define IEEE80211_NODE_REQ_HT20     0x00400000      /* HT20 requesting node */
#define IEEE80211_NODE_ATH_PAUSED 0x00800000         /* all the tid queues in ath layer are paused*/ 
#define IEEE80211_NODE_UAPSD_CREDIT_UPDATE 0x01000000  /*Require credit update*/

/* NB: These must have the same values as IEEE80211_ATHC_* */
#define IEEE80211_NODE_TURBOP   0x01          /* Turbo prime enable */
#define IEEE80211_NODE_COMP     0x02          /* Compresssion enable */
#define IEEE80211_NODE_FF       0x04          /* Fast Frame capable */
#define IEEE80211_NODE_XR       0x08          /* Atheros WME enable */
#define IEEE80211_NODE_AR       0x10          /* AR capable */
#define IEEE80211_NODE_BOOST    0x80 

/*============================================================================= */
/*                          PUBLIC TYPE DEFINITIONS */
/*============================================================================= */
typedef enum
{
    LOCATE_SUCCESS,
    ADD_SUCCESS,
    DEL_SUCCESS,
    POWER_SUCCESS,
    STATE_SUCCESS,
    NOT_INITIALIZED,
    LOCATE_FAILURE,
    STATION_EXISTS_ERROR,
    TABLE_ACCESS_ERROR,
    TABLE_FULL_ERROR,
    INVALID_STATE_ERROR,
    NOT_AUTHENTICATED,
    NOT_ASSOCIATED,
    ALREADY_AUTHENTICATED,
    ALREADY_ASSOCIATED,
    ALREADY_AUTHENTICATING,
    ALREADY_ASSOCIATING,
    ALREADY_DEASSOCIATING,
    ALREADY_REASSOCIATING,
#ifdef WPA
    RSN_IE_BUF_OVERFLOW,
#endif
} extStaDb_Status_e;
/* */
/* The set of possible status responses to operations performed on the table */
/* */

typedef enum
{
    UNAUTHENTICATED = 0,
    SME_INIT_AUTHENTICATING,
    EXT_INIT_AUTHENTICATING,
    AUTHENTICATED,
    SME_INIT_DEAUTHENTICATING,
    EXT_INIT_DEAUTHENTICATING,
    SME_INIT_ASSOCIATING,
    EXT_INIT_ASSOCIATING,
    ASSOCIATED,
    SME_INIT_REASSOCIATING,
    EXT_INIT_REASSOCIATING,
    SME_INIT_DEASSOCIATING,
    EXT_INIT_DEASSOCIATING,
    /*Milind. 10/10/05. To maintain peer states for when the*/
    /*WB associates/joins*/
    WB_ASSOCIATED,
    WB_ADHOC_JOINED
} extStaDb_State_e;
typedef UINT8 extStaDb_State_t;
/* */
/* The possible states a station or AP can have with respect to the */
/* station containing the external station table */
/* */

typedef enum
{
    BONLY_MODE,
    GONLY_MODE,
    MIXED_MODE,
    AONLY_MODE,
    NONLY_MODE,
} extStaDb_ClientMode_e;

typedef enum
{
    MYFALSE,
    MYTRUE
}Boolean;

#ifdef IEEE80211H
typedef struct basic_info_t
{
    BOOLEAN capability;
    UINT32 mToken;
    UINT8 measureStartTime[8];  /* indicates if the Sta is a QSTA */
    UINT16 measureDuration;     /* the duration in which the measurement takes place */
    UINT16 measuredChannel;     /* channel the station is currently measuring */   
    IEEEtypes_MeasurementRepMap_t mMap; /* the result of measurement report */
} basic_info;

typedef struct tcc_info_t
{
    BOOLEAN capability;
    UINT32 mToken;
    UINT8 measureStartTime[8];  /* indicates if the Sta is a QSTA */
    UINT16 measureDuration;     /* the duration in which the measurement takes place */
    UINT16 measuredChannel;     /* channel the station is currently measuring */    
} tcc_info;

typedef struct rpi_info_t
{
    BOOLEAN capability;
    UINT32 mToken;
    UINT8 measureStartTime[8];  /* indicates if the Sta is a QSTA */
    UINT16 measureDuration;     /* the duration in which the measurement takes place */
    UINT16 measuredChannel;     /* channel the station is currently measuring */   
} rpi_info;

typedef struct extStaDb_measurement_info_t
{
    UINT32 DiaglogToken;
    basic_info mBasic;
    tcc_info mTcc;
    rpi_info mRpi;    
} extStaDb_measurement_info_t;

#endif /* IEEE80211h */
typedef struct Frag11n_t
{
#ifdef ZERO_COPY
    struct sk_buff *skb[5];
#else
    struct sk_buff *skb;
#endif
    UINT8 *curPosition_p;
    UINT8 status;
    UINT8 status_pre;
    UINT8 pad;
    UINT16 length;
    UINT32 jiffies;
}Frag11n;
#ifdef DYNAMIC_BA_SUPPORT
typedef struct txACInfo_t
{
    UINT32 txa_avgpps;
    UINT32 txa_lastsample;
    UINT32 txa_pkts;
}txACInfo;
#endif
#define MAX_AGG_QUE 4
#define MAX_TIDS    8
typedef struct Aggr11n_t
{
    Frag11n Frag[MAX_AGG_QUE];
    UINT8 cap;  
    UINT8 index;
    UINT8 on;
    UINT8 start;
    UINT32 txcnt;
    UINT16 threshold;
    UINT8 queon;
    UINT8 nextpktnoaggr;
    spinlock_t       Lock;             /* used to protect aggregation*/
    struct sk_buff_head   txQ;
    UINT16 thresholdBackUp;     
    UINT32 txcntbytid[8];
    UINT8 onbytid[8];
    UINT8 startbytid[8];
    UINT8 type;
#ifdef DYNAMIC_BA_SUPPORT
    txACInfo tx_ac_info[MAX_TIDS];
#endif
}Aggr11n;
typedef struct
{
    struct sk_buff  *pFrame;
    UINT16 SeqNo;
    UINT8 FragNo;
}
DeFragBufInfo_t;

#ifdef SOC_W8764
typedef struct dbRateInfo_t
{
    UINT32 Format:      1; //0 = Legacy format, 1 = Hi-throughput format
    UINT32 ShortGI:     1; //0 = Use standard guard interval,1 = Use short guard interval
    UINT32 Bandwidth:   1; //0 = Use 20 MHz channel,1 = Use 40 MHz channel
    UINT32 RateIDMCS:   7;
    UINT32 AdvCoding:   1; //ldpc
    UINT32 AntSelect:   2; //Bitmap to select one of the transmit antennae
    UINT32 ActSubChan:  2; //Active subchannel for 40 MHz mode 00:lower, 01= upper, 10= both on lower and upper
    UINT32 Preambletype: 1; //Preambletype 0= Long, 1= Short;
}dbRateInfo_t;

typedef struct RssiPathInfo_t
{
    UINT32 a:   8;
    UINT32 b:   8;
    UINT32 c:   8;
    UINT32 d:   8;
}PACK RssiPathInfo_t;

#else
typedef struct dbRateInfo_t
{
    UINT16  Format:     1;  //0 = Legacy format, 1 = Hi-throughput format
    UINT16  ShortGI:    1;  //0 = Use standard guard interval,1 = Use short guard interval
    UINT16  Bandwidth:  1;  //0 = Use 20 MHz channel,1 = Use 40 MHz channel
    UINT16  RateIDMCS:  6;  //= RateID[3:0]; Legacy format,= MCS[5:0]; HT format
    UINT16  AdvCoding:  2;  //AdvCoding 0 = No AdvCoding,1 = LDPC,2 = RS,3 = Reserved
    UINT16  AntSelect:  2;  //Bitmap to select one of the transmit antennae
    UINT16  ActSubChan: 2;   //Active subchannel for 40 MHz mode 00:lower, 01= upper, 10= both on lower and upper
    UINT16  Preambletype:1;  //Preambletype 0= Long, 1= Short;

}dbRateInfo_t;
#endif

typedef struct extStaDb_StaInfo_t
{
    IEEEtypes_MacAddr_t Addr;
    IEEEtypes_MacAddr_t Bssid;
    BOOLEAN AP;
    BOOLEAN SmeInitiated;
    extStaDb_State_t State;
    IEEEtypes_PwrMgmtMode_t PwrMode;
    UINT16 StnId;
    UINT16 Aid;
    UINT32  FwStaPtr;
    UINT32 TimeStamp;
    UINT16 QueueToUse;
    UINT8 ClientMode;
    /*add by zhanxuechao*/
    UINT8 ni_flags;               /* special-purpose state */
    /* new State Machine housekeeping */
    AssocSrvSta   mgtAssoc;
    AuthRspSrvSta mgtAuthRsp;
    AuthReqSrvSta mgtAuthReq;
    PowerSaveMonitor pwrSvMon;
    keyMgmthsk_hsm_t keyMgmtHskHsm;
#ifdef STA_INFO_DB
    UINT8    Sq2;              /* Signal Quality 2 */
    UINT8    Sq1;              /* Signal Quality 1 */
    UINT8    Rate;             /* rate at which frame was received */
    UINT8    RSSI;             /* RF Signal Strength Indicator */
#endif
    UINT8  ApMode;
#ifdef QOS_FEATURE
    UINT8 IsStaQSTA; //indicates if the Sta is a QSTA
    UINT8 MoreDataAck;//Sta can preocess Ack frames with MoreDataBit in FrmCtl to 1
#endif
#ifdef IEEE80211H
    UINT16 ListenInterval;     /* Listen interveal */
    BOOLEAN IsSpectrumMgmt;    /* Is capable to do spectrum management */
    extStaDb_measurement_info_t measureInfo;
#endif /* IEEE80211H */
    keyMgmtInfo_t keyMgmtStateInfo; //keyMgmtInfo;
    //keyMgmt_StateInfo_t keyMgmtStateInfo;//keep this as the last element in this data structure
#ifdef APCFGUR
    RemoteCtrlSrv rmSrv;
    UINT8   UR;
#endif
#ifdef PPPoE_SUPPORT
    UINT16  Session_ID;
    //  UINT8   IamWaiting;
#endif
#ifdef WMM_PS_SUPPORT
    QoS_WmeInfo_Info_t    Qosinfo;  /** store apsd qosinfo **/
#endif
    Aggr11n aggr11n;
    DeFragBufInfo_t DeFragBufInfo;
    IEEEtypes_HT_Element_t HtElem;
    IEEEtypes_Add_HT_Element_t AddHtElme;
    dbRateInfo_t    RateInfo;
#ifdef WDS_FEATURE
    void    *wdsInfo;
    void    *wdsPortInfo;
#endif
    BOOLEAN     Client;
    UINT8 IsStaMSTA;
#ifdef MRVL_WSC //MRVL_WSC_IE
    WSC_ProbeRespIE_t WscIEBuf ;
    UINT8 WSCSta;
#endif
    MIB_802DOT11 *mib_p;
    struct net_device *dev;
    IEEEtypes_HT_Cap_t PeerHTCapabilitiesInfo;
    UINT8 StaType;
#ifdef SOC_W8764
    RssiPathInfo_t RSSI_path;
#endif
    /*lirui add*/
    struct ieee80211_stastats sta_stats;
    /*add by zhanxuechao for mac binding*/
    UINT16 ni_vid;
    UINT8  ni_ip_auto_set;
    UINT8  ni_ipaddr[4];
    unsigned long ni_assocuptime;
    IEEEtypes_CapInfo_t CapInfo;
    /*add by zhanxuechao for online traffic process*/
    struct timer_list    online_traffic_timer;
    int         lowest_traffic_limit_switch;
    UINT32      lowest_traffic_limit_timelength;
    UINT64      lowest_traffic_limit_threshold;
    UINT32      ns_time_remain;            /*  rx data count (bytes) of last time */
    struct autelan_traffic_limit ni_everynode_tl;   //Added by duanmingzhe for traffic limit 2.0//lhj//2012.06
    struct autelan_traffic_limit ni_special_tl;     //Added by duanmingzhe for traffic limit 2.0//lhj//2012.06

#ifdef AE5200_RT_SUPPORT
	/*zhaoyang add for data forward base users*/
	u_int16_t ni_localflags;
	u_int8_t ni_portal_ipaddr[4];
	/*zhaoyang modify end*/
#endif
}
extStaDb_StaInfo_t;
#define EXT_STA_TABLE_SIZE  MAX_STNS 

typedef struct ExtStaInfoItem_t
{
    struct ExtStaInfoItem_t *nxt; /* part of List Object node elments */
    struct ExtStaInfoItem_t *prv;
    struct ExtStaInfoItem_t *nxt_ht; /* for hashtable list */
    struct ExtStaInfoItem_t *prv_ht;
    extStaDb_StaInfo_t StaInfo; /* information data */
}
ExtStaInfoItem_t;

typedef struct eth_StaInfo_t
{
    IEEEtypes_MacAddr_t Addr;
    struct extStaDb_StaInfo_t *pStaInfo_t;
    UINT32 TimeStamp;
} eth_StaInfo_t;

typedef struct EthStaItem_t
{
    struct EthStaItem_t *nxt; /* part of List Object node elments */
    struct EthStaItem_t *prv;
    struct EthStaItem_t *nxt_ht; /* for hashtable list */
    struct EthStaItem_t *prv_ht;
    eth_StaInfo_t ethStaInfo; /* information data */
} EthStaItem_t;

struct STADB_CTL
{
    BOOLEAN Initialized;
    UINT16 MaxStaSupported; 
    ExtStaInfoItem_t *ExtStaInfoDb; 
    ExtStaInfoItem_t *ExtStaInfoDb_p[EXT_STA_TABLE_SIZE];
    List FreeStaList;
    List StaList;
    int aging_time_in_minutes;
};

struct ETHSTADB_CTL
{
    BOOLEAN eInitialized;
    EthStaItem_t *EthStaDb; 
    EthStaItem_t *EthStaDb_p[EXT_STA_TABLE_SIZE];
    List FreeEthStaList;
    List EthStaList;
    int aging_time_in_minutes;
};

/* */
/* The information recorded for an external station: */
/* 1) Its MAC address */
/* 2) Whether or not it is an AP */
/* 3) The state with respect to the station containing this database */
/* 4) The power mode of the external station */
/* */
#define AGING_TIMER_VALUE_IN_SECONDS   10 

/*============================================================================= */
/*                    PUBLIC PROCEDURES (ANSI Prototypes) */
/*============================================================================= */

/******************************************************************************
*
* Name: extStaDb_Init
*
* Description:
*    Routine to initial the structures used in the external stations table.
*
* Conditions For Use:
*    None.
*
* Arguments:
*    None.
*
* Return Value:
*    Status indicating success or failure.
*
* Notes:
*    None.
*
*****************************************************************************/
extern WL_STATUS extStaDb_Init(vmacApInfo_t *vmacSta_p, UINT16 MaxStns );

/******************************************************************************
*
* Name: extStaDb_AddSta
*
* Description:
*    This routine adds a station to the external station table.
*
* Conditions For Use:
*    External station table has been initialized.
*
* Arguments:
*    Arg1 (i  ): StaInfo - Pointer to a structure containing information
*                          about the station being added
*
* Return Value:
*    Status indicating the results of the operation.
*
* Notes:
*    None.
*
*****************************************************************************/
extern extStaDb_Status_e extStaDb_AddSta (vmacApInfo_t *vmacSta_p, extStaDb_StaInfo_t *StaInfo_p);

/******************************************************************************
*
* Name: extStaDb_DelSta
*
* Description:
*    This routine deletes a station from the external station table.
*
* Conditions For Use:
*    External station table has been initialized.
*
* Arguments:
*    Arg1 (i  ): Addr_p  - Pointer to the MAC address of the station to be
*                          deleted
*
* Return Value:
*    Status indicating the results of the operation.
*
* Notes:
*    None.
*
*****************************************************************************/
extern extStaDb_Status_e extStaDb_DelSta (vmacApInfo_t *vmacSta_p, IEEEtypes_MacAddr_t *Addr_p, int option);

/******************************************************************************
*
* Name: extStaDb_GetStaInfo
*
* Description:
*    This routine attempts to retrieve the state for the given MAC address.
*
* Conditions For Use:
*    External station table has been initialized.
*
* Arguments:
*    Arg1 (i  ): Addr_p  - Pointer to the MAC address of the station for
*                          which the state is to be retrieved
*    Arg2 (  o): StaInfo_p - Pointer to the variable that will contain the
*                          requested station information
*
* Return Value:
*    Status indicating the results of the operation.
*
* Notes:
*    None.
*
*****************************************************************************/
extern extStaDb_Status_e extStaDb_GetState2(vmacApInfo_t *vmacSta_p, IEEEtypes_MacAddr_t *Addr_p,
                                            extStaDb_State_e *State_p );


extern extStaDb_StaInfo_t  *extStaDb_GetStaInfo(vmacApInfo_t *vmacSta_p, IEEEtypes_MacAddr_t *Addr_p,
                                                int option);

UINT32 Wang32BitMix( UINT32 Key );

#define STADB_DONT_UPDATE_AGINGTIME 0
#define STADB_UPDATE_AGINGTIME      1
#define STADB_NO_BLOCK              3
/* We assume all Tx packets length is 1512. 
throughput under AGGRTHRESHOLD 2000 is about 8*1512*2000 = 24192000 bps */
#define AGGRTHRESHOLD              2000

#define AGGKEEPNUM 40
/******************************************************************************
*
* Name: extStaDb_SetPwrMode
*
* Description:
*    This routine attempts to set the given power mode for the given MAC
*    address.
*
* Conditions For Use:
*    External station table has been initialized.
*
* Arguments:
*    Arg1 (i  ): Addr_p  - Pointer to the MAC address of the station for
*                          which a power mode update is to be made
*    Arg2 (i  ): NewMode - The new power mode of the station
*
* Return Value:
*    Status indicating the results of the operation.
*
* Notes:
*    None.
*
*****************************************************************************/
extern extStaDb_Status_e extStaDb_SetPwrMode(vmacApInfo_t *vmacSta_p,
                                             IEEEtypes_MacAddr_t *Addr_p,
                                             IEEEtypes_PwrMgmtMode_e NewMode );

/******************************************************************************
*
* Name: extStaDb_GetPwrMode
*
* Description:
*    This routine attempts to retrieve the power mode for the given MAC
*    address.
*
* Conditions For Use:
*    External station table has been initialized.
*
* Arguments:
*    Arg1 (i  ): Addr_p - Pointer to the MAC address of the station for
*                         which the power mode is to be retrieved
*    Arg2 (  o): Mode_p - Pointer to the variable that will contain the
*                         requested power mode information
*
* Return Value:
*    Status indicating the results of the operation.
*
* Notes:
*    None.
*
*****************************************************************************/
extern extStaDb_Status_e extStaDb_GetPwrMode(vmacApInfo_t *vmacSta_p,
                                             IEEEtypes_MacAddr_t *Addr_p,
                                             IEEEtypes_PwrMgmtMode_e *Mode_p );

extern extStaDb_Status_e extStaDb_SetAid(vmacApInfo_t *vmacSta_p,
                                         IEEEtypes_MacAddr_t *Addr_p,
                                         UINT16 Aid);

extern extStaDb_Status_e extStaDb_GetStnId(vmacApInfo_t *vmacSta_p,
                                           IEEEtypes_MacAddr_t *Addr_p,
                                           UINT16 *StnId_p );
extern UINT16 extStaDb_entries( vmacApInfo_t *vmacSta_p,UINT8 );
extern int set_sta_aging_time(vmacApInfo_t *vmacSta_p,int minutes);
extern int set_sta_auth_aging_time(vmacApInfo_t *vmacSta_p,int minutes);        //zhouke add,set auth agingtime;
extern void extStaDb_AgingTimerInit(vmacApInfo_t *vmacSta_p);
extern void extStaDb_ProcessKeepAliveTimerInit(vmacApInfo_t *vmacSta_p);
extern UINT16 extStaDb_list(vmacApInfo_t *vmacSta_p, UINT8 *buf , UINT8);

#ifdef WPA
extern extStaDb_Status_e extStaDb_SetRSNDataTrafficEnabled(vmacApInfo_t *vmacSta_p,
                                                           IEEEtypes_MacAddr_t *Addr_p, UINT8 value );
extStaDb_Status_e extStaDb_SetRSNPwk(vmacApInfo_t *vmacSta_p,IEEEtypes_MacAddr_t *Addr_p,
                                     UINT8* pEncryptKey, UINT32* pTxMICKey, UINT32* pRxMICKey);
extStaDb_Status_e extStaDb_SetRSNPwkAndDataTraffic(vmacApInfo_t *vmacSta_p,
                                                   IEEEtypes_MacAddr_t *Addr_p,
                                                   UINT8* pEncryptKey, UINT32* pTxMICKey, UINT32* pRxMICKey );
extStaDb_Status_e extStaDb_GetRSNPwk(vmacApInfo_t *vmacSta_p,IEEEtypes_MacAddr_t *Addr_p,
                                     UINT8* pEncryptKey, UINT32* pTxMICKey, UINT32* pRxMICKey);
extStaDb_Status_e extStaDb_SetRSNPmk(vmacApInfo_t *vmacSta_p,IEEEtypes_MacAddr_t *Addr_p, UINT8* pPMK );
extStaDb_Status_e extStaDb_GetKeyMgmtInfo(vmacApInfo_t *vmacSta_p,IEEEtypes_MacAddr_t *Addr_p, keyMgmtInfo_t *KeyMgmtInfo);
extStaDb_Status_e extStaDb_SetKeyMgmtInfo(vmacApInfo_t *vmacSta_p,IEEEtypes_MacAddr_t *Addr_p, keyMgmtInfo_t *KeyMgmtInfo);
//extStaDb_Status_e extStaDb_SetTimerData(IEEEtypes_MacAddr_t *Addr_p,
//                                        timer_Data_t *tData);
extStaDb_Status_e extStaDb_GetRSN_IE(vmacApInfo_t *vmacSta_p, IEEEtypes_MacAddr_t *Addr_p,
                                     UINT8 *RsnIE_p );
extStaDb_Status_e extStaDb_SetRSN_IE(vmacApInfo_t *vmacSta_p, IEEEtypes_MacAddr_t *Addr_p,
                                     IEEEtypes_RSN_IE_t *RsnIE_p );
#ifdef MRVL_WSC
extStaDb_Status_e extStaDb_GetWSC_IE(vmacApInfo_t *vmacSta_p,  IEEEtypes_MacAddr_t *Addr_p,
                                     UINT8 *WscIE_p );
#endif
extern extStaDb_Status_e extStaDb_GetPairwiseTSC(vmacApInfo_t *vmacSta_p, IEEEtypes_MacAddr_t *Addr_p,
                                                 UINT32 *pTxIV32, UINT16 *pTxIV16 );
extern extStaDb_Status_e extStaDb_SetPairwiseTSC(vmacApInfo_t *vmacSta_p, IEEEtypes_MacAddr_t *Addr_p,
                                                 UINT32 TxIV32, UINT16 TxIV16 );
extStaDb_Status_e extStaDb_GetStaInfoAndKeys(vmacApInfo_t *vmacSta_p,
                                             IEEEtypes_MacAddr_t *Addr_p,
                                             extStaDb_StaInfo_t *StaInfo_p,
                                             int AgingTimeMode,
                                             PacketType_e mode);
extStaDb_Status_e extStaDb_GetStaInfoAndTxKeys(vmacApInfo_t *vmacSta_p, IEEEtypes_MacAddr_t *, extStaDb_StaInfo_t *, UINT32);
extStaDb_Status_e extStaDb_GetStaInfoAndRxKeys(vmacApInfo_t *vmacSta_p, IEEEtypes_MacAddr_t *, extStaDb_StaInfo_t *, UINT32);
//extStaDb_Status_e extStaDb_SetKeyMgmtState(IEEEtypes_MacAddr_t *Addr_p,
//        keyMgmtState_e State);
//extStaDb_Status_e extStaDb_SetTimerData(IEEEtypes_MacAddr_t *Addr_p,
//                                    timer_Data_t *tData);
extStaDb_Status_e extStaDb_SetPhase1Key(vmacApInfo_t *vmacSta_p,IEEEtypes_MacAddr_t *Addr_p,
                                        UINT16 *Phase1Key,
                                        PacketType_e mode, UINT32 RxIV32);
void extStaDb_SendGrpKeyMsgToAllSta(vmacApInfo_t *vmacSta_p);
void extStaDb_SetNewState4AllSta(vmacApInfo_t *vmacSta_p,extStaDb_State_e NewState);
extStaDb_StaInfo_t * extStaDb_GetStaInfoWPA(vmacApInfo_t *vmacSta_p,IEEEtypes_MacAddr_t *Addr_p,
                                            UINT32 AgingTimeMode);
extern void extStaDb_ProcessAgeEvt(vmacApInfo_t *vmacSta_p);
extern extStaDb_Status_e extStaDb_SetQoSOptn(vmacApInfo_t *vmacSta_p,
                                             IEEEtypes_MacAddr_t *,
                                             UINT8 );
extern UINT8 extStaDb_GetQoSOptn(vmacApInfo_t *vmacSta_p,IEEEtypes_MacAddr_t *Addr_p);
extern void extStaDb_RemoveAllStns(vmacApInfo_t *vmacSta_p,UINT16 Reason);
extern extStaDb_Status_e extStaDb_GetWMM_DeliveryEnableInfo(vmacApInfo_t *vmacSta_p,
                                                            IEEEtypes_MacAddr_t *,
                                                            UINT8 , UINT8 *, UINT8 *);

extern UINT8 extStaDb_Check_Uapsd_Capability(vmacApInfo_t *vmacSta_p,
                                             IEEEtypes_MacAddr_t *);
extern UINT8 extStaDb_Check_ALL_AC_DeliveryEnableInfo(vmacApInfo_t *vmacSta_p,
                                                      IEEEtypes_MacAddr_t *);
extern extStaDb_Status_e extStaDb_GetMeasurementInfo(vmacApInfo_t *vmacSta_p,
                                                     IEEEtypes_MacAddr_t *,
                                                     extStaDb_measurement_info_t *);
extern extStaDb_Status_e extStaDb_SetMeasurementInfo(vmacApInfo_t *vmacSta_p,
                                                     IEEEtypes_MacAddr_t *,
                                                     extStaDb_measurement_info_t *);



#endif
extern UINT16 extStaDb_AggrFrameCk(vmacApInfo_t *vmacSta_p, int force);
extern void Disable_extStaDb_ProcessKeepAliveTimer(vmacApInfo_t *vmacSta_p);
extern WL_STATUS ethStaDb_Init(vmacApInfo_t *vmacSta_p, UINT16 MaxStns );
extern extStaDb_Status_e ethStaDb_AddSta (vmacApInfo_t *vmac_p, IEEEtypes_MacAddr_t *Addr_p, extStaDb_StaInfo_t *StaInfo_p);
extern eth_StaInfo_t  *ethStaDb_GetStaInfo(vmacApInfo_t *vmac_p, IEEEtypes_MacAddr_t *Addr_p, int option);
extern void ethStaDb_RemoveAllStns(vmacApInfo_t *vmac_p);
extern extStaDb_Status_e ethStaDb_RemoveSta(vmacApInfo_t *vmac_p, IEEEtypes_MacAddr_t *Addr_p);
extern extStaDb_Status_e ethStaDb_RemoveStaPerWlan(vmacApInfo_t *vmac_p, IEEEtypes_MacAddr_t *Addr_p);

/*add by zhanxuechao*/
extern void ieee80211_node_authorize(extStaDb_StaInfo_t *StaInfo_p,vmacApInfo_t *vmacSta_p);
extern void ieee80211_node_unauthorize(extStaDb_StaInfo_t *StaInfo_p);
extern int ieee80211_node_is_authorize(extStaDb_StaInfo_t *StaInfo_p);
extern UINT16 extStaDb_online_traffic_set_timer(vmacApInfo_t *vmac_p);
#endif /* _STADB_H */
