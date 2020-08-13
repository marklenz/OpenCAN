/* lincan.h
 * Header file for the Linux CAN-bus driver.
 * Written by Arnaud Westenberg email:arnaud@wanadoo.nl
 * Rewritten for new CAN queues by Pavel Pisa - OCERA team member
 * email:pisa@cmp.felk.cvut.cz
 * Bug fixes and improvements by Andrew Trzeciak
 * email:andrewt@advancedweb.com
 * This software is released under the GPL-License.
 * Version lincan-0.3.1.1  28 Oct 2004
 */

#ifndef _CAN_DRVAPI_T_H
#define _CAN_DRVAPI_T_H

#ifdef __KERNEL__

#include <linux/time.h>
#include <linux/types.h>
#include <linux/ioctl.h>

#else /* __KERNEL__ */

#include <sys/time.h>
#include <sys/types.h>
#include <sys/ioctl.h>

#endif /* __KERNEL__ */

#ifdef __cplusplus
extern "C" {
#endif

/* 
 * CAN_MSG_VERSION_2 enables new canmsg_t layout compatible with
 * can4linux project from http://www.port.de/
 * 
 */
#define CAN_MSG_VERSION_2

/* Number of data bytes in one CAN message */
#define CAN_MSG_LENGTH 8

#ifdef CAN_MSG_VERSION_2

typedef struct timeval canmsg_tstamp_t ;

/**
 * struct canmsg_t - structure representing CAN message
 * @flags:  message flags
 *      %MSG_RTR .. message is Remote Transmission Request,
 *	%MSG_EXT .. message with extended ID, 
 *      %MSG_OVR .. indication of queue overflow condition,
 *	%MSG_LOCAL .. message originates from this node.
 * @cob:    communication object number (not used)
 * @id:     ID of CAN message
 * @timestamp: not used
 * @length: length of used data
 * @data:   data bytes buffer
 *
 * Header: canmsg.h
 */
struct canmsg_t {
	int             flags;
	int             cob;
	unsigned long   id;
	canmsg_tstamp_t timestamp;
	unsigned short  length;
	unsigned char   data[CAN_MSG_LENGTH];
};

#else /*CAN_MSG_VERSION_2*/
#ifndef PACKED
#define PACKED __attribute__((packed))
#endif

typedef unsigned long canmsg_tstamp_t ;

/* Old, deprecated version of canmsg_t structure */
struct canmsg_t {
	short		flags;
	int		cob;
	unsigned long	id;
	canmsg_tstamp_t	timestamp;
	unsigned int	length;
	unsigned char	data[CAN_MSG_LENGTH];
} PACKED;
#endif /*CAN_MSG_VERSION_2*/

typedef struct canmsg_t canmsg_t;

/**
 * struct canfilt_t - structure for acceptance filter setup
 * @flags:  message flags
 *      %MSG_RTR .. message is Remote Transmission Request,
 *	%MSG_EXT .. message with extended ID, 
 *      %MSG_OVR .. indication of queue overflow condition,
 *	%MSG_LOCAL .. message originates from this node.
 *	there are corresponding mask bits
 *	%MSG_RTR_MASK, %MSG_EXT_MASK, %MSG_LOCAL_MASK.
 *	%MSG_PROCESSLOCAL enables local messages processing in the
 *	combination with global setting
 * @queid:  CAN queue identification in the case of the multiple
 *	    queues per one user (open instance)
 * @cob:    communication object number (not used)
 * @id:     selected required value of cared ID id bits
 * @mask:   select bits significand for the comparation;
 *          1 .. take care about corresponding ID bit, 0 .. don't care
 *
 * Header: canmsg.h
 */
struct canfilt_t {
	int		flags;
	int		queid;
	int		cob;
	unsigned long	id;
	unsigned long	mask;
};

typedef struct canfilt_t canfilt_t;

/* Definitions to use for canmsg_t and canfilt_t flags */
#define MSG_RTR   (1<<0)	/* message is Remote Transmission Request */
#define MSG_OVR   (1<<1)	/* indication of queue overflow condition */
#define MSG_EXT   (1<<2)	/* message with extended ID */
#define MSG_LOCAL (1<<3)	/* message originates from this node */
/* If you change above lines, check canque_filtid2internal function */

/* Additional definitions used for canfilt_t only */
#define MSG_FILT_MASK_SHIFT   8
#define MSG_RTR_MASK   (MSG_RTR<<MSG_FILT_MASK_SHIFT)
#define MSG_EXT_MASK   (MSG_EXT<<MSG_FILT_MASK_SHIFT)
#define MSG_LOCAL_MASK (MSG_LOCAL<<MSG_FILT_MASK_SHIFT)
#define MSG_PROCESSLOCAL (MSG_OVR<<MSG_FILT_MASK_SHIFT)

/* Can message ID mask */
#define MSG_ID_MASK ((1l<<29)-1)

/* CAN ioctl magic number */
#define CAN_IOC_MAGIC 'd'

typedef unsigned long bittiming_t;
typedef unsigned short channel_t;

/* bus timing register 0 */
#define BTR0_SJW_MASK	(3)

/* bus timing register 1 */
#define BTR1_SAM 		(1<<1)		/* The Bus is Sampled 1=three times or 0=once */

/* struct can_stats_t - flags */
#define CAN_STATS_RESET	(1<<0)	/* after copying stats reset all values to zero */

/* struct can_stats_t - error count */
#define ERR_CNT_DIR(n)	(((n)&1)<<0)
#define ERR_CNT_TYPE(n)	(((n)&3)<<1)

#define ERR_CNT_RX		ERR_CNT_DIR(1)

#define ERR_CNT_BIT		ERR_CNT_TYPE(0)
#define ERR_CNT_FORM	ERR_CNT_TYPE(1)
#define ERR_CNT_STUFF	ERR_CNT_TYPE(2)
#define ERR_CNT_OTHER	ERR_CNT_TYPE(3)

struct can_stats_t {
	unsigned long flags;			/* flags for stats */
	unsigned long rxq_dropped;		/* number of dropped Rx messages when RX queue is full */
	unsigned long overrun_error;	/* can chip Rx queue overrun erros */
	unsigned long sr;				/* last captured SR (STATUS REGISTER) */
	unsigned long eccr;				/* last captured ECCR (ERROR CODE CAPTURE REGISTER) */
	unsigned long error_count[8];
};
typedef struct can_stats_t can_stats_t;

/**
 * struct can_baudparams_t - datatype for calling CONF_BAUDPARAMS IOCTL 
 * @flags: BTR1_SAM
 * @baudrate: baud rate in Hz
 * @sjw: synchronization jump width (0-3) prescaled clock cycles
 * @sampl_pt: sample point in % (0-100) sets (TSEG1+1)/(TSEG1+TSEG2+2) ratio
 * 
 * The structure is used to configure new set of parameters into CAN controller chip.
 * If default value of some field should be preserved, fill field by value -1.
 */
struct can_baudparams_t {
	long flags;			/* defualt  0 */
	long baudrate;		/* defualt  250000 = 250kbps */
	long sjw;			/* defualt  1 */
	long sample_pt;		/* defualt  8750 = 87.5% */
};
typedef struct can_baudparams_t can_baudparams_t;

struct can_accfilter_t {
	unsigned long mask;			/* default 0xFFFFFFFF */
	unsigned long code;			/* default 0x00000000 */
};
typedef struct can_accfilter_t can_accfilter_t;

/* CAN ioctl functions */
#define CAN_DRV_QUERY _IO(CAN_IOC_MAGIC, 0)
#define CAN_DRV_QRY_BRANCH    0	/* returns driver branch value - "LINC" for LinCAN driver */
#define CAN_DRV_QRY_VERSION   1	/* returns driver version as (major<<16) | (minor<<8) | patch */
#define CAN_DRV_QRY_MSGFORMAT 2	/* format of canmsg_t structure */

#define CMD_START _IOW(CAN_IOC_MAGIC, 1, channel_t)
#define CMD_STOP _IOW(CAN_IOC_MAGIC, 2, channel_t)
#define CMD_RESET _IOW(CAN_IOC_MAGIC, 3, channel_t)

#define CONF_BTREG _IOW(CAN_IOC_MAGIC, 4, bittiming_t)

#define CMD_STATUS _IOR(CAN_IOC_MAGIC, 8, can_stats_t)
#define CMD_DUMPREG _IO(CAN_IOC_MAGIC, 9)

#define CANQUE_FILTER _IOW(CAN_IOC_MAGIC, 10, canfilt_t)
#define CANQUE_FLUSH _IO(CAN_IOC_MAGIC, 11)

#define CONF_BAUDPARAMS _IOW(CAN_IOC_MAGIC, 12, can_baudparams_t)
#define CONF_ACCFILT _IOW(CAN_IOC_MAGIC, 13, can_accfilter_t)

#ifdef __cplusplus
} /* extern "C"*/
#endif

#endif /*_CAN_DRVAPI_T_H*/
