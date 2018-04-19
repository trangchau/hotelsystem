/*
	HotelPMS�@���ʒ�`
*/

#ifndef	pms_defH
#define	pms_defH

#include <afx.h>
#include <stdlib.h>
#include ".\Common\bsc_def.h"
#include ".\Common\sys_def.h"
#include ".\Common\lnk_mng.h"
#include ".\Common\ini_inf.h"
#include ".\Common\dbs_mng.h"

#define		FLV_TRUE				1						// 
#define		FLV_FALSE				0						// 

#define		FCK_HOTEL				1						// �h��
#define		FCK_SPA					2						// ����E���C
#define		FCK_SPORTS				3						// �X�|�[�c�{��
#define		FCK_SHOP				4						// �V���b�v
#define		FCK_RESTAURANT			5						// ���X�g����
#define		FCK_CASINO				6						// �J�W�m

#define		EQK_ROOM				1						// �����i�L�[�j
#define		EQK_LOCKER				2						// ���b�J�[�i�L�[�j
#define		EQK_MASSAGE				10						// �}�b�T�[�W
#define		EQK_GOLF				20						// �S���t(���b�J�[)
#define		EQK_TABLE				30						// ���X�g�����E�J�W�m(�e�[�u��)

#define		EPK_PROPER				1						// ���K�ٗp�Ј�
#define		EPK_FULLTIME			2						// �񐳋K�ٗp�Ј��i�t���^�C���j
#define		EPK_PARTTIME			3						// �񐳋K�ٗp�Ј��i�p�[�g�^�C���j
#define		EPK_OTHER				4						// ���̑�

#define		DRK_MERCHANDISE			1						// ��ʔ̔����i
#define		DRK_SERVICE				2						// �T�[�r�X�𖱏��i
#define		DRK_TAX					3						// �ŋ�
#define		DRK_ADDITIONAL			4						// �t�ї���
#define		DRK_DEPOSIT				5						// �a���/�O���
#define		DRK_DISCOUNT			6						// �l��/����
#define		DRK_EXTRA_SERVICE		7						// �ǉ��T�[�r�X���i
#define		DRK_TRANSFER			9						// ���{�݂ւ̐U��

#define		PRK_AMOUNT				1						// ���z(Cash)
#define		PRK_RATE				2						// ����(Rate)
#define		PRK_DISC_AMOUNT			3						// �l����(Amount discount)
#define		PRK_DISC_RATE			4						// ������(Rate discount)

#define		GDR_MALE				1						// �j��(male)
#define		GDR_FEMALE				2						// ����(female)
#define		GDR_VOID				0						// �s���E�s�L�ځE�s��(void)

#define		CLK_PERSONAL			1						// �l�q(Personal client)
#define		CLK_GROUP				2						// �c�̋q(Group client)

#define		CLL_VIP					1						// Very Important Person
#define		CLL_DOMINANT			2						// Dominant Person
#define		CLL_GENERAL				3						// General Person
#define		CLL_UG					9						// Undesirable Guest
#define		CLL_DEFAULT				(CLL_GENERAL)			// Default rank

#define		AGK_REAL				1						// Agent has real shops
#define		AGK_ONLINE				2						// Online salse only

#define		CDK_BUSINESS			1						// �c�Ɠ�(Business day)
#define		CDK_HOLIDAY				2						// �x��(Holiday)
#define		CDK_PRE_HOLIDAY			3						// �x�O��(Before holiday)

#define		CLS_CLEAN				1						// �N���[�j���O��(Cleaning complete)
#define		CLS_DIRTY				2						// �N���[�j���O��(Dirty)
#define		CLS_INSPECTED			3						// �����m�F��(Inspected)
#define		CLS_PICKUP				4						// �ŏI�m�F(finally room checking)

#define		USS_VACANT				0						// ��
#define		USS_OCCUPIED			1						// �g�p��
//#define		USS_INSPECT				2						// ������

#define		RSS_ENABLE				0						// enable to sale the room 
#define		RSS_OUTOFORDER			1						// room is out of order
#define		RSS_OUTOFSERVICE		2						// room is out of service

#define		AGS_ADULT				1						// ��l
#define		AGS_KIDS				2						// ���l
#define		AGS_BABY				3						// �c��

#define		EQA_NORTH_WING			0x1						// �k�E�B���O
#define		EQA_EAST_WING			0x2						// ���E�B���O
#define		EQA_WEST_WING			0x4						// ���E�B���O
#define		EQA_SOUTH_WING			0x8						// ��E�B���O
#define		EQA_NORTH_DIRECTION		0x10					// �k��// 
#define		EQA_EAST_DIRECTION		0x20					// ����// 
#define		EQA_WEST_DIRECTION		0x40					// ����// 
#define		EQA_SOUTH_DIRECTION		0x80					// �쑤// 
#define		EQA_OCEAN_VIEW			0x100					// �C��// 
#define		EQA_MOUNTAIN_VIEW		0x200					// �R��// 
#define		EQA_CITY_VIEW			0x400					// �X��// 
#define		EQA_LARGE_SIZE			0x1000					// ��^(Lage locker)
#define		EQA_NORMAL_SIZE			0x2000					// �W���T�C�Y(Tipical locker)
#define		EQA_SMALL_SIZE			0x4000					// ���^(Small locker)

#define		EQA_SMOKING				0x1						// �i��
#define		EQA_NOSMOKING			0x2						// �։�

#define		PAYMENT_CASH			1						// Payment by cash
#define		PAYMENT_CREDITCARD		2						// Payment by creditcard
#define		PAYMENT_TRANSFER		3						// Payment by bank transfer
#define		PAYMENT_COUPON			4						// Payment by coupon
#define		PAYMENT_DEPOSIT			5						// Payment by deposit
#define		PAYMENT_RECEIVABLE		6						// Payment by Accounts receivable
#define		PAYMENT_OTHER			99						// Payment by other method

#define		VIA_AGENT				0x1						// Reservation via agent
#define		VIA_WEBSITE				0x2						// Resrevation via direct WebSite
#define		VIA_TELEPHONE			0x4						// Reservation via tel.
#define		VIA_FAX					0x8						// Reservation via fax
#define		VIA_EMAIL				0x10					// Reservation via direct email
#define		VIA_WALKIN				0x20					// Walkin checkin
#define		VIA_DIRECT				(VIA_TELEPHONE|VIA_FAX|VIA_EMAIL)		// Reservation direct

#define		DKD_SUNDAY				0x1						// ���j��
#define		DKD_MONDAY				0x2						// ���j��
#define		DKD_TUESDAY				0x4						// �Ηj��
#define		DKD_WEDNESDAY			0x8						// ���j��
#define		DKD_THURSDAY			0x10					// �ؗj��
#define		DKD_FRIDAY				0x20					// ���j��
#define		DKD_SATURDAY			0x40					// �y�j��
#define		DKD_HOLIDAY				0x80					// �x��
#define		DKD_PRE_HOLIDAY			0x100					// �x�O��


#define		CGK_ONCE				1						// �P�񗘗p����
#define		CGK_TIME				2						// �P���ԗ��p����
#define		CGK_EXTEND_TIME			3						// ���������i�P���ԒP�ʁj

#define		CGP_HOTEL_STAY			((EQK_ROOM<<8)+1)		// �z�e���h��
#define		CGP_HOTEL_DAYUSE		((EQK_ROOM<<8)+2)		// �z�e�����A��
#define		CGP_HOTEL_RESIDENCE		((EQK_ROOM<<8)+10)		// �z�e�����Z
#define		CGP_HOTEL_EARLY_CHECKIN	((EQK_ROOM<<8)+15)		// �z�e�������`�F�b�N�C��
#define		CGP_HOTEL_LATE_CHECKOUT	((EQK_ROOM<<8)+16)		// �z�e���x���`�F�b�N�A�E�g
#define		CGP_GOLF_NORMAL			((EQK_GOLF<<8)+1)		// �S���t��ʏ�g�p
#define		CGP_GOLF_RESERVED		((EQK_GOLF<<8)+2)		// �S���t��ݐ؎g�p
#define		CGP_GOLF_MEMBER			((EQK_GOLF<<8)+4)		// �S���t�ꃁ���o�[�g�p
#define		CGP_GOLF_VISITOR		((EQK_GOLF<<8)+8)		// �S���t��r�W�^�[�g�p
#define		CGP_LOCKER_NORMAL		((EQK_LOCKER<<8)+1)		// ���b�J�[�ʏ�g�p
#define		CGP_MASSERGE_NORMAL		((EQK_MASSAGE<<8)+1)	// �}�b�T�[�W�ʏ�g�p



#define		ALF_HOTEL				0x1						// �z�e���@�\
#define		ALF_RESTAURANT			0x2						// ���X�g�����i���W�j�@�\
#define		ALF_GOLF				0x4						// �S���t�@�\
#define		ALF_SPORTS				0x8						// �X�|�[�c�{�݋@�\
#define		ALF_SPA					0x10					// ����E�}�b�T�[�W�@�\

#define		OPF_REGISTER			0x1						// PC���W�@�\
#define		OPF_TRANSFER_HOTEL		0x2						// �z�e���U�֋@�\

#define		PCS_ROOM				0x1						// �������ɂ��ۋ�
#define		PCS_FACILITY			0x2						// �{�ݐ��ɂ��ۋ�
#define		PCS_BRANCH				0x4						// �c�Ə����ɂ��ۋ�
#define		PCS_OPERATOR			0x1000					// �^�c�ҒP�ʂ̉ۋ�

#define		PMS_NOT_PAID			0						// ���x��
#define		PMS_PAID				1						// �x����

#define		CIS_BEFORE_CHECKIN		0						// �`�F�b�N�C���O
#define		CIS_INHOUSE				1						// �`�F�b�N�C����
#define		CIS_AFTER_CHECKOUT		2						// �`�F�b�N�A�E�g��

#define		APPLY_EXTEND_CHARGE		0x1						// ���������K�p
#define		FORCE_DEPOSIT_COLLECT	0x2						// �a�����������
#define		NEED_CONVENTANT			0x4						// ���񏑈���v
#define		SEND_CONFIRMATION_EMAIL	0x8						// �\��m�F���[�����M

//#define		LANG_ENGLISH			_T("EN")				// �p��
//#define		LANG_VIETNAMESE			_T("VN")				// �x�g�i����
//#define		LANG_JAPANESE			_T("JP")				// ���{��

#define		OPK_CLAIM				1						// �N���[��
#define		OPK_OPINION				2						// �ӌ�
#define		OPK_CANCEL_REASON		9						// �L�����Z�����R


#define		ACM_GOVERNMENT			1						// ���{�쐬
#define		ACM_USER				2						// ���[�U�[�쐬

#define		DCK_RSV_CONFIRMATION	1						// �\��m�F���[���p
#define		DCK_CONVENTANT			2						// �`�F�b�N�C�����̐���

#define		CCC_LOGIN_FAILURES_TIMES	1					// �`�F�b�N�C�����s�\��
#define		CCC_AUTO_LOGOUT_MINUTES		2					// �������O�A�E�g����
#define		CCC_MAIL_SERVER_INFORMATION	100					// Suzutek���[�����M�T�[�o�[���

#define		DPK_DEPOSIT				1						// deposit_kind(DEPOSIT)
#define		DPK_PAYMENT				2						// deposit_kind(PAYMENT)
#define		DPK_REFUND				3						// deposit_kind(REFUND)

typedef	struct	{							// operator_infos segment
	int				id[DBF_SIZ_INT];		// Record ID
	WCHAR			cod[DBF_SIZ_WST(30)];	// operator_code
	WCHAR			nam[DBF_SIZ_WST(30)];	// name
	WCHAR			abr[DBF_SIZ_WST(30)];	// abbreviation
	WCHAR			adr1[DBF_SIZ_WST(255)];	// address1
	WCHAR			adr2[DBF_SIZ_WST(255)];	// address2
	WCHAR			tel[DBF_SIZ_WST(255)];	// telephone_number
	WCHAR			fax[DBF_SIZ_WST(255)];	// fax_number
	WCHAR			url[DBF_SIZ_WST(255)];	// web_url
	WCHAR			eml[DBF_SIZ_WST(255)];	// email_address
	WCHAR			lip[DBF_SIZ_WST(255)];	// logo_image_path
	WCHAR			tkn[DBF_SIZ_WST(255)];	// CM_user_token
	int				asb[DBF_SIZ_INT];		// accounting_subject_kind
	int				acm[DBF_SIZ_INT];		// accounting_method
	int				adi[DBF_SIZ_INT];		// accounting_department_id
	int				mei[DBF_SIZ_INT];		// modefied_employee
	Day				cdt[DBF_SIZ_DTM];		// created_date(datetime)
	Day				mdt[DBF_SIZ_DTM];		// modified_date(datetime)
	Day				ddt[DBF_SIZ_DTM];		// deleted_date(datetime)
}OPR_INF;

typedef	struct	{							// branch_infos segment
	int				id[DBF_SIZ_INT];		// id
	WCHAR			opc[DBF_SIZ_WST(30)];	// operator_code
	WCHAR			brc[DBF_SIZ_WST(255)];	// branch_code
	WCHAR			nam[DBF_SIZ_WST(255)];	// name
	WCHAR			abr[DBF_SIZ_WST(255)];	// abbreviation
	WCHAR			adr1[DBF_SIZ_WST(255)];	// address1
	WCHAR			adr2[DBF_SIZ_WST(255)];	// address2
	WCHAR			tel[DBF_SIZ_WST(255)];	// telephone_number
	WCHAR			fax[DBF_SIZ_WST(255)];	// fax_number
	WCHAR			url[DBF_SIZ_WST(30)];	// web_url
	WCHAR			eml[DBF_SIZ_WST(30)];	// email_address
	int				cms[DBF_SIZ_INT];		// company_store
	WCHAR			lgp[DBF_SIZ_WST(30)];	// logo_image_path
	int				cpi[DBF_SIZ_INT];		// CM_property_id
	int				ask[DBF_SIZ_INT];		// accounting_subject_kind
	int				acm[DBF_SIZ_INT];		// accounting_method
	int				adi[DBF_SIZ_INT];		// accounting_department_id
	int				mei[DBF_SIZ_INT];		// modified_employee_id
	Day				cdt[DBF_SIZ_DTM];		// created_date(datetime)
	Day				mdt[DBF_SIZ_DTM];		// modified_date(datetime)
	Day				ddt[DBF_SIZ_DTM];		// deleted_date(datetime)
}BRC_INF;

typedef	struct	{							// facility_info segment
	int				id[DBF_SIZ_INT];		// id
	WCHAR			opc[DBF_SIZ_WST(30)];	// operator_code
	WCHAR			brc[DBF_SIZ_WST(30)];	// branch_code
	WCHAR			fcc[DBF_SIZ_WST(30)];	// facility_code
	WCHAR			nam[DBF_SIZ_WST(255)];	// name
	WCHAR			abr[DBF_SIZ_WST(255)];	// abbreviation
	int				hve[DBF_SIZ_INT];		// have_equipments
	Day				cit[DBF_SIZ_TIM];		// normal_time_check_in
	Day				cot[DBF_SIZ_TIM];		// normal_time_check_out
	int				eci[DBF_SIZ_INT];		// exemption_early_check_in
	int				lco[DBF_SIZ_INT];		// exemption_late_check_out
	WCHAR			adr1[DBF_SIZ_WST(255)];	// address1
	WCHAR			adr2[DBF_SIZ_WST(255)];	// address2
	WCHAR			tel[DBF_SIZ_WST(255)];	// telephone_number
	WCHAR			fax[DBF_SIZ_WST(255)];	// fax_number
	WCHAR			url[DBF_SIZ_WST(255)];	// web_url
	WCHAR			eml[DBF_SIZ_WST(255)];	// email_address
	int				cms[DBF_SIZ_INT];		// company_store
	int				fck[DBF_SIZ_INT];		// facility_kind
	int				fcl[DBF_SIZ_INT];		// facility_color
	int				opt[DBF_SIZ_INT];		// optional_configuration
	int				dcc[DBF_SIZ_INT];		// day_close_charge
	int				adi[DBF_SIZ_INT];		// accounting_department_id
	int				mei[DBF_SIZ_INT];		// modified_employee_id
	Day				cdt[DBF_SIZ_DTM];		// created_date(datetime)
	Day				mdt[DBF_SIZ_DTM];		// modified_date(datetime)
	Day				ddt[DBF_SIZ_DTM];		// deleted_date(datetime)
}FCL_INF;

typedef	struct	{							// facility_bulding_infos segment
	int				id[DBF_SIZ_INT];		// id
	WCHAR			opc[DBF_SIZ_WST(30)];	// operator_code
	WCHAR			brc[DBF_SIZ_WST(30)];	// branch_code
	WCHAR			fcc[DBF_SIZ_WST(30)];	// facility_code
	WCHAR			nam[DBF_SIZ_WST(255)];	// building_name
	WCHAR			abr[DBF_SIZ_WST(255)];	// abbreviation
	int				mei[DBF_SIZ_INT];		// modified_employee_id
	Day				cdt[DBF_SIZ_DTM];		// created_date(datetime)
	Day				mdt[DBF_SIZ_DTM];		// modified_date(datetime)
	Day				ddt[DBF_SIZ_DTM];		// deleted_date(datetime)
}BLD_INF;

typedef	struct	{							// duties segment
	int				id[DBF_SIZ_INT];		// id
	WCHAR			opc[DBF_SIZ_WST(30)];	// operator_code
	WCHAR			brc[DBF_SIZ_WST(30)];	// branch_code
	WCHAR			fcc[DBF_SIZ_WST(30)];	// facility_code
	WCHAR			nam[DBF_SIZ_WST(255)];	// name
	WCHAR			abr[DBF_SIZ_WST(255)];	// abbreviation
	int				odr[DBF_SIZ_INT];		// display_order
	int				mei[DBF_SIZ_INT];		// modified_employee_id
	Day				cdt[DBF_SIZ_DTM];		// created_date(datetime)
	Day				mdt[DBF_SIZ_DTM];		// modified_date(datetime)
	Day				ddt[DBF_SIZ_DTM];		// deleted_date(datetime)
}DTY_INF;

typedef	struct	{							// Employees segment
	int				id[DBF_SIZ_INT];		// id
	WCHAR			opc[DBF_SIZ_WST(30)];	// operator_code
	int				uii[DBF_SIZ_INT];		// user_id
	WCHAR			epc[DBF_SIZ_WST(30)];	// employee_code
	WCHAR			brc[DBF_SIZ_WST(30)];	// branch_code
	WCHAR			fcc[DBF_SIZ_WST(30)];	// facility_code
	WCHAR			fnm[DBF_SIZ_WST(255)];	// first_name
	WCHAR			lnm[DBF_SIZ_WST(255)];	// last_name
	int				gdr[DBF_SIZ_INT];		// gender
	Day				bth[DBF_SIZ_DAY];		// date_of_birth
	int				mrd[DBF_SIZ_INT];		// married
	WCHAR			adr1[DBF_SIZ_WST(255)];	// address1
	WCHAR			adr2[DBF_SIZ_WST(255)];	// address2
	WCHAR			zip[DBF_SIZ_WST(255)];	// zip_code
	WCHAR			tel1[DBF_SIZ_WST(255)];	// telephone_number1
	WCHAR			tel2[DBF_SIZ_WST(255)];	// telephone_number2
	WCHAR			eml1[DBF_SIZ_WST(255)];	// email_address1
	WCHAR			eml2[DBF_SIZ_WST(255)];	// email_address2
	WCHAR			nto[DBF_SIZ_WST(255)];	// nationality
	WCHAR			lng[DBF_SIZ_WST(255)];	// language
	WCHAR			slg[DBF_SIZ_WST(255)];	// system_language
	WCHAR			idn[DBF_SIZ_WST(255)];	// id_number
	Day				isd[DBF_SIZ_DAY];		// date_of_issue_of_id
	Day				exd[DBF_SIZ_DAY];		// expiration_date_of_id
	Day				epd[DBF_SIZ_DAY];		// employment_date
	int				rli[DBF_SIZ_INT];		// role_id
	int				dti[DBF_SIZ_INT];		// duties_id
	int				knd[DBF_SIZ_INT];		// employee_kind
	WCHAR			pth[DBF_SIZ_WST(255)];	// photo_image_path
	WCHAR			pwd[DBF_SIZ_WST(255)];	// login password
	int				lck[DBF_SIZ_INT];		// locked
	int				mei[DBF_SIZ_INT];		// modified_employee_id
	Day				cdt[DBF_SIZ_DTM];		// created_date(datetime)
	Day				mdt[DBF_SIZ_DTM];		// modified_date(datetime)
	Day				ddt[DBF_SIZ_DTM];		// deleted_date(datetime)
}EMP_LYE;

typedef	struct	{							// equipment_infos segment
	int				id[DBF_SIZ_INT];		// id
	WCHAR			opc[DBF_SIZ_WST(30)];	// operator_code
	WCHAR			brc[DBF_SIZ_WST(30)];	// branch_code
	WCHAR			fcc[DBF_SIZ_WST(30)];	// facility_code
	Day				dte[DBF_SIZ_DAY];		// date_of_calendar
	int				knd[DBF_SIZ_INT];		// calendar_kind
	WCHAR			cmt[DBF_SIZ_WST(30)];	// comment
	int				mei[DBF_SIZ_INT];		// modified_employee_id
	Day				cdt[DBF_SIZ_DTM];		// created_date(datetime)
	Day				mdt[DBF_SIZ_DTM];		// modified_date(datetime)
	Day				ddt[DBF_SIZ_DTM];		// deleted_date(datetime)
}BIZ_CDR;

typedef	struct	{							// business_calendar ( for reference )
	Day				dte[1];					// date of calendar
	int				knd;					// kind of day
}BIZ_CDR_DAT;

typedef	struct	{							// equipment_infos segment
	int				id[DBF_SIZ_INT];		// id
	WCHAR			opc[DBF_SIZ_WST(30)];	// operator_code
	WCHAR			brc[DBF_SIZ_WST(30)];	// branch_code
	WCHAR			fcc[DBF_SIZ_WST(30)];	// facility_code
	int				ekd[DBF_SIZ_INT];		// equipment_kind
	int				eti[DBF_SIZ_INT];		// equipment_type_id
	int				fbi[DBF_SIZ_INT];		// facility_building_info_id
	WCHAR			eco[DBF_SIZ_WST(255)];	// equipment_code
	int				adi1[DBF_SIZ_INT];		// additional_info1
	int				adi2[DBF_SIZ_INT];		// additional_info2
	int				adi3[DBF_SIZ_INT];		// additional_info3
	WCHAR			nam[DBF_SIZ_WST(255)];	// name
	WCHAR			abr[DBF_SIZ_WST(255)];	// abbreviation
	int				rcp[DBF_SIZ_INT];		// regular_capacity
	int				ecp[DBF_SIZ_INT];		// extra_capacity
	int				sle[DBF_SIZ_INT];		// sale_enable
	int				epi[DBF_SIZ_INT];		// employee_id
	int				mei[DBF_SIZ_INT];		// modified_employee_id
	Day				cdt[DBF_SIZ_DTM];		// created_date(datetime)
	Day				mdt[DBF_SIZ_DTM];		// modified_date(datetime)
	Day				ddt[DBF_SIZ_DTM];		// deleted_date(datetime)
}EQP_INF;

typedef	struct	{							// equipment_types segment
	int				id[DBF_SIZ_INT];		// id
	WCHAR			opc[DBF_SIZ_WST(30)];	// operator_code
	WCHAR			brc[DBF_SIZ_WST(30)];	// branch_code
	WCHAR			fcc[DBF_SIZ_WST(30)];	// facility_code
	int				ekd[DBF_SIZ_INT];		// equipment_kind
	WCHAR			nam[DBF_SIZ_WST(255)];	// name
	WCHAR			abr[DBF_SIZ_WST(255)];	// abbreviation
	int				cmo[DBF_SIZ_INT];		// CM_occupancy
	int				cmi[DBF_SIZ_INT];		// CM_room_myallocator_id
	int				cmu[DBF_SIZ_INT];		// CM_unit
	int				mei[DBF_SIZ_INT];		// modified_employee_id
	Day				cdt[DBF_SIZ_DTM];		// created_date(datetime)
	Day				mdt[DBF_SIZ_DTM];		// modified_date(datetime)
	Day				ddt[DBF_SIZ_DTM];		// deleted_date(datetime)
}EQP_TYP;

typedef	struct	{							// agent_infos segment
	int				id[DBF_SIZ_INT];		// id
	WCHAR			opc[DBF_SIZ_WST(30)];	// operator_code
	WCHAR			agc[DBF_SIZ_WST(30)];	// agent_code
	WCHAR			nam[DBF_SIZ_WST(255)];	// name
	WCHAR			abr[DBF_SIZ_WST(255)];	// abbreviation
	int				agk[DBF_SIZ_INT];		// agent_kind
	WCHAR			adr1[DBF_SIZ_WST(255)];	// address1
	WCHAR			adr2[DBF_SIZ_WST(255)];	// address2
	WCHAR			zip[DBF_SIZ_WST(255)];	// zip_code
	WCHAR			tel1[DBF_SIZ_WST(255)];	// telephone_number1
	WCHAR			tel2[DBF_SIZ_WST(255)];	// telephone_number2
	WCHAR			fax[DBF_SIZ_WST(255)];	// fax_number
	WCHAR			eml1[DBF_SIZ_WST(255)];	// email_address1
	WCHAR			eml2[DBF_SIZ_WST(255)];	// email_address2
	WCHAR			url[DBF_SIZ_WST(255)];	// url
	WCHAR			ctr[DBF_SIZ_WST(255)];	// country
	int				rct[DBF_SIZ_INT];		// reservation_count
	int				etc[DBF_SIZ_INT];		// establishment_count
	int				cxc[DBF_SIZ_INT];		// cancel_count
	int				cmo[DBF_SIZ_INT];		// CM_ota_channel_code
	int				mei[DBF_SIZ_INT];		// modified_employee_id
	Day				cdt[DBF_SIZ_DTM];		// created_date(datetime)
	Day				mdt[DBF_SIZ_DTM];		// modified_date(datetime)
	Day				ddt[DBF_SIZ_DTM];		// deleted_date(datetime)
}AGT_INF;

typedef	struct	{							// creditcard_companies_infos segment
	int				id[DBF_SIZ_INT];		// id
	WCHAR			opc[DBF_SIZ_WST(30)];	// operator_code
	WCHAR			ccc[DBF_SIZ_WST(30)];	// creditcard_company_code
	WCHAR			nam[DBF_SIZ_WST(255)];	// name
//	WCHAR			abr[DBF_SIZ_WST(255)];	// abbreviation
	WCHAR			adr1[DBF_SIZ_WST(255)];	// address1
	WCHAR			adr2[DBF_SIZ_WST(255)];	// address2
	WCHAR			zip[DBF_SIZ_WST(255)];	// zip_code
	WCHAR			tel1[DBF_SIZ_WST(255)];	// telephone_number1
	WCHAR			tel2[DBF_SIZ_WST(255)];	// telephone_number2
	WCHAR			fax[DBF_SIZ_WST(255)];	// fax_number
	WCHAR			eml1[DBF_SIZ_WST(255)];	// email_address1
	WCHAR			eml2[DBF_SIZ_WST(255)];	// email_address2
	WCHAR			url[DBF_SIZ_WST(255)];	// url
	double			rte[DBF_SIZ_DBL];		// fee rate
	WCHAR			ctr[DBF_SIZ_WST(255)];	// country
	Day				pdy[DBF_SIZ_INT];		// payment_day
	Day				cld[DBF_SIZ_INT];		// closing_day
	int				mei[DBF_SIZ_INT];		// modified_employee_id
	Day				cdt[DBF_SIZ_DTM];		// created_date(datetime)
	Day				mdt[DBF_SIZ_DTM];		// modified_date(datetime)
	Day				ddt[DBF_SIZ_DTM];		// deleted_date(datetime)
}CDC_COM;

typedef	struct	{							// market_segments
	int				id[DBF_SIZ_INT];		// id
	WCHAR			opc[DBF_SIZ_WST(30)];	// operator_code
	WCHAR			brc[DBF_SIZ_WST(30)];	// branch_code
	WCHAR			fcc[DBF_SIZ_WST(30)];	// facility_code
	WCHAR			nam[DBF_SIZ_WST(255)];	// name
	WCHAR			abr[DBF_SIZ_WST(255)];	// abbreviation
	WCHAR			cmt[DBF_SIZ_WST(255)];	// comment
	int				mei[DBF_SIZ_INT];		// modified_employee_id
	Day				cdt[DBF_SIZ_DTM];		// created_date
	Day				mdt[DBF_SIZ_DTM];		// modified_date
	Day				ddt[DBF_SIZ_DTM];		// deleted_date
}MKT_SEG;

typedef	struct	{							// client_infos segment
	int				id[DBF_SIZ_INT];		// id
	WCHAR			opc[DBF_SIZ_WST(30)];	// operator_code
	int				ckd[DBF_SIZ_INT];		// client_kind
	WCHAR			fnm[DBF_SIZ_WST(255)];	// first_name
	WCHAR			lnm[DBF_SIZ_WST(255)];	// last_name
	int				gdr[DBF_SIZ_INT];		// gender
	Day				bth[DBF_SIZ_DTM];		// date_of_birth
	WCHAR			adr1[DBF_SIZ_WST(255)];	// address1
	WCHAR			adr2[DBF_SIZ_WST(255)];	// address2
	WCHAR			zip[DBF_SIZ_WST(255)];	// zip_code
	WCHAR			tel1[DBF_SIZ_WST(255)];	// telephone_number1
	WCHAR			tel2[DBF_SIZ_WST(255)];	// telephone_number2
	WCHAR			fax[DBF_SIZ_WST(255)];	// fax_number
	WCHAR			eml1[DBF_SIZ_WST(255)];	// email_address1
	WCHAR			eml2[DBF_SIZ_WST(255)];	// email_address2
	WCHAR			nto[DBF_SIZ_WST(255)];	// nationality
	WCHAR			lng[DBF_SIZ_WST(255)];	// language
	WCHAR			slg[DBF_SIZ_WST(255)];	// system_language
	int				ccv[DBF_SIZ_INT];		// currency_conversion_id
	WCHAR			idn[DBF_SIZ_WST(255)];	// id_number
	Day				isd[DBF_SIZ_DTM];		// date_of_issue_of_id
	Day				exd[DBF_SIZ_DTM];		// expiration_date_of_id
	WCHAR			vsn[DBF_SIZ_WST(255)];	// visa_number
	Day				vex[DBF_SIZ_DTM];		// expiration_date_of_visa
	int				crk[DBF_SIZ_INT];		// client_rank
	int				mrd[DBF_SIZ_INT];		// married
	int				fve1[DBF_SIZ_INT];		// favorite_equipment1
	int				fve2[DBF_SIZ_INT];		// favorite_equipment2
	int				usc[DBF_SIZ_INT];		// usage_count
	int				nsc[DBF_SIZ_INT];		// noshow_count
	WCHAR			pip[DBF_SIZ_WST(255)];	// photo_image_path
	int				ism[DBF_SIZ_INT];		// is_smoker
	WCHAR			mbi[DBF_SIZ_WST(255)];	// member_id
	WCHAR			txc[DBF_SIZ_WST(255)];	// tax_code
	int				ccc[DBF_SIZ_INT];		// creditcard_company_id
	WCHAR			ccn[DBF_SIZ_WST(255)];	// creditcard_number
	WCHAR			cct[DBF_SIZ_WST(255)];	// creditcard_title
	int				mei[DBF_SIZ_INT];		// modified_employee_id
	Day				cdt[DBF_SIZ_DTM];		// created_date(datetime)
	Day				mdt[DBF_SIZ_DTM];		// modified_date(datetime)
	Day				ddt[DBF_SIZ_DTM];		// deleted_date(datetime)
}CLI_INF;

typedef	struct	{							// currency_conversions segment
	int				id[DBF_SIZ_INT];		// id
	WCHAR			opc[DBF_SIZ_WST(30)];	// operator_code
	WCHAR			brc[DBF_SIZ_WST(30)];	// branch_code
	WCHAR			fcc[DBF_SIZ_WST(30)];	// facility_code
	WCHAR			crc[DBF_SIZ_WST(255)];	// currency_code
	WCHAR			nam[DBF_SIZ_WST(255)];	// name
	int				isb[DBF_SIZ_INT];		// is_base
	int				iss[DBF_SIZ_INT];		// is_secondly
	int				adp[DBF_SIZ_INT];		// after_dec_point
	double			rte[DBF_SIZ_DBL];		// exchange_rate
	int				mei[DBF_SIZ_INT];		// modified_employee_id
	Day				cdt[DBF_SIZ_DTM];		// created_date(datetime)
	Day				mdt[DBF_SIZ_DTM];		// modified_date(datetime)
	Day				ddt[DBF_SIZ_DTM];		// deleted_date(datetime)
}CRC_CVS;

typedef	struct	{							// reservation_infos segment
	int				id[DBF_SIZ_INT];		// record id
	WCHAR			opc[DBF_SIZ_WST(30)];	// operator_code
	WCHAR			brc[DBF_SIZ_WST(30)];	// branch_code
	WCHAR			fcc[DBF_SIZ_WST(30)];	// facility_code
	int				rvn[DBF_SIZ_INT];		// reservation_number
	int				cii[DBF_SIZ_INT];		// client_info_id
	WCHAR			rpm[DBF_SIZ_WST(255)];	// representative_name
	int				rmg[DBF_SIZ_INT];		// representative_gender
	int				aii[DBF_SIZ_INT];		// agent_info_id
	int				olr[DBF_SIZ_INT];		// online_reservation
	WCHAR			ern[DBF_SIZ_WST(255)];	// external_reservation_number
	Day				arv[DBF_SIZ_DAY];		// first_arrival_date
	Day				dpt[DBF_SIZ_DAY];		// last_departure_date
	int				cxl[DBF_SIZ_INT];		// canceled
	int				nsw[DBF_SIZ_INT];		// no_show
	int				cis[DBF_SIZ_INT];		// check_in_status
	int				bfr[DBF_SIZ_INT];		// path_of_reservation( booking from )
	WCHAR			nte[DBF_SIZ_WST(255)];	// note
	Day				rcd[DBF_SIZ_DAY];		// reservation_create_date
	int				msi[DBF_SIZ_INT];		// market_segment_id
	int				pym[DBF_SIZ_INT];		// payment_method
	int				mei[DBF_SIZ_INT];		// modefied_employee
	Day				cdt[DBF_SIZ_DTM];		// created_date(datetime)
	Day				mdt[DBF_SIZ_DTM];		// modified_date(datetime)
	Day				ddt[DBF_SIZ_DTM];		// deleted_date(datetime)
}RSV_INF;


typedef	struct	{							// reservation_details segment
	int				id[DBF_SIZ_INT];		// id
	WCHAR			opc[DBF_SIZ_WST(30)];	// operator_code
	int				rii[DBF_SIZ_INT];		// reservation_info_id
	int				cii[DBF_SIZ_INT];		// client_info_id
	Day				arv[DBF_SIZ_DAY];		// arrival_date
	Day				atm[DBF_SIZ_TIM];		// arrival_time
	Day				dpt[DBF_SIZ_DAY];		// departure_date
	Day				dtm[DBF_SIZ_TIM];		// departure_time
	Day				cid[DBF_SIZ_DTM];		// check_in_date
	Day				cod[DBF_SIZ_DTM];		// check_out_date
	int				eci[DBF_SIZ_INT];		// early_check_in
	int				lco[DBF_SIZ_INT];		// late_check_out
	int				psn[DBF_SIZ_INT];		// person_number
	int				chn[DBF_SIZ_INT];		// children_number
	int				eti[DBF_SIZ_INT];		// equipment_type_id
	int				inh[DBF_SIZ_INT];		// inhouse
	int				cxl[DBF_SIZ_INT];		// canceled
	WCHAR			nte[DBF_SIZ_WST(1024)];	// note
	int				mei[DBF_SIZ_INT];		// modefied_employee
	Day				cdt[DBF_SIZ_DTM];		// created_date(datetime)
	Day				mdt[DBF_SIZ_DTM];		// modified_date(datetime)
	Day				ddt[DBF_SIZ_DTM];		// deleted_date(datetime)
}RSV_DTL;

typedef	struct	{							// reservation_equipments segment
	int				id[DBF_SIZ_INT];		// id
	WCHAR			opc[DBF_SIZ_WST(30)];	// operator_code
	int				rii[DBF_SIZ_INT];		// reservation_info_id
	int				rdi[DBF_SIZ_INT];		// reservation_detail_id
	int				eti[DBF_SIZ_INT];		// equipment_type_id
	int				eii[DBF_SIZ_INT];		// equipment_info_id
	Day				usd[DBF_SIZ_DAY];		// use_start_date
	Day				ued[DBF_SIZ_DAY];		// use_end_date
	int				inh[DBF_SIZ_INT];		// inhouse
	Day				lud[DBF_SIZ_DAY];		// last_used_date
	int				dsi[DBF_SIZ_INT];		// description_id
	double			aam[DBF_SIZ_DBL];		// actual_amount
	int				mei[DBF_SIZ_INT];		// modified_employee_id
	Day				cdt[DBF_SIZ_DTM];		// created_date(datetime)
	Day				mdt[DBF_SIZ_DTM];		// modified_date(datetime)
	Day				ddt[DBF_SIZ_DTM];		// deleted_date(datetime)
}RSV_EQP;

typedef	struct	{							// reservation_charges segment
	int				id[DBF_SIZ_INT];		// id
	WCHAR			opc[DBF_SIZ_WST(30)];	// operator_code
	int				rii[DBF_SIZ_INT];		// reservation_info_id
	int				rdi[DBF_SIZ_INT];		// reservation_detail_id
	int				rei[DBF_SIZ_INT];		// reservation_equipment_id
	Day				udt[DBF_SIZ_DAY];		// use_date
	int				cgk[DBF_SIZ_INT];		// charge_kind
	int				dsi[DBF_SIZ_INT];		// description_id
	int				edi[DBF_SIZ_INT];		// equipment_charge_details_id
	double			amt[DBF_SIZ_DBL];		// actual_amount
	int				pup[DBF_SIZ_INT];		// paid_up
	int				mei[DBF_SIZ_INT];		// modified_employee_id
	Day				cdt[DBF_SIZ_DTM];		// created_date(datetime)
	Day				mdt[DBF_SIZ_DTM];		// modified_date(datetime)
	Day				ddt[DBF_SIZ_DTM];		// deleted_date(datetime)
}RSV_CRG;

typedef	struct	{							// descriptions segment
	int				id[DBF_SIZ_INT];		// id
	WCHAR			opc[DBF_SIZ_WST(30)];	// operator_code
	WCHAR			brc[DBF_SIZ_WST(30)];	// branch_code
	WCHAR			fcc[DBF_SIZ_WST(30)];	// facility_code
	WCHAR			dsc[DBF_SIZ_WST(255)];	// description_code
	int				dkd[DBF_SIZ_INT];		// description_kind
	int				dci[DBF_SIZ_INT];		// description_category_id
	WCHAR			nam[DBF_SIZ_WST(255)];	// name
	WCHAR			abr[DBF_SIZ_WST(255)];	// abbreviation
	Day				vss[DBF_SIZ_DAY];		// valid_sale_period_start
	Day				vse[DBF_SIZ_DAY];		// valid_sale_period_end
	int				prk[DBF_SIZ_INT];		// price_kind
	double			prc[DBF_SIZ_INT];		// price
	int				apl[DBF_SIZ_INT];		// tax_application
	int				atr[DBF_SIZ_INT];		// tax_attribution
	int				sen[DBF_SIZ_INT];		// sale_enable
	WCHAR			rac[DBF_SIZ_WST(255)];	// readable_code
	int				tfi[DBF_SIZ_INT];		// transfer_origin_facility_id
	int				mei[DBF_SIZ_INT];		// modified_employee_id
	Day				cdt[DBF_SIZ_DTM];		// created_date(datetime)
	Day				mdt[DBF_SIZ_DTM];		// modified_date(datetime)
	Day				ddt[DBF_SIZ_DTM];		// deleted_date(datetime)
}DSC_RPT;


typedef	struct	{							// description_categories segment
	int				id[DBF_SIZ_INT];		// id
	WCHAR			opc[DBF_SIZ_WST(30)];	// operator_code
	WCHAR			brc[DBF_SIZ_WST(30)];	// branch_code
	WCHAR			fcc[DBF_SIZ_WST(30)];	// facility_code
	WCHAR			nam[DBF_SIZ_WST(255)];	// name
	WCHAR			abr[DBF_SIZ_WST(255)];	// abbreviation
	int				odr[DBF_SIZ_INT];		// display_order
	int				mei[DBF_SIZ_INT];		// modified_employee_id
	Day				cdt[DBF_SIZ_DTM];		// created_date
	Day				mdt[DBF_SIZ_DTM];		// modified_date
	Day				ddt[DBF_SIZ_DTM];		// deleted_date
}DSC_CAT;

typedef	struct	{							// equipment_charge_infos segment
	int				id[DBF_SIZ_INT];		// id
	WCHAR			opc[DBF_SIZ_WST(30)];	// operator_code
	WCHAR			brc[DBF_SIZ_WST(30)];	// branch_code
	WCHAR			fcc[DBF_SIZ_WST(30)];	// facility_code
	int				eti[DBF_SIZ_INT];		// equipment_type_id
	int				eii[DBF_SIZ_INT];		// equipment_info_id
	int				aii[DBF_SIZ_INT];		// agent_info_id
	int				dsi[DBF_SIZ_INT];		// description_id
	int				cgk[DBF_SIZ_INT];		// charge_kind
	int				cgp[DBF_SIZ_INT];		// charge_particulars
	int				bfr[DBF_SIZ_INT];		// reservation_route
	int				psn[DBF_SIZ_INT];		// persons
	int				cmr[DBF_SIZ_INT];		// CM_rooms_unit
	int				cmn[DBF_SIZ_INT];		// CM_minstay
	int				cmx[DBF_SIZ_INT];		// CM_maxstay
	int				mei[DBF_SIZ_INT];		// modified_employee_id
	Day				cdt[DBF_SIZ_DTM];		// created_date(datetime)
	Day				mdt[DBF_SIZ_DTM];		// modified_date(datetime)
	Day				ddt[DBF_SIZ_DTM];		// deleted_date(datetime)
}EQP_CRG;

typedef	struct	{							// equipment_charge_details segment
	int				id[DBF_SIZ_INT];		// id
	WCHAR			opc[DBF_SIZ_WST(30)];	// operator_code
	WCHAR			brc[DBF_SIZ_WST(30)];	// branch_code
	WCHAR			fcc[DBF_SIZ_WST(30)];	// facility_code
	int				eci[DBF_SIZ_INT];		// equipment_charge_info_id
	double			prc[DBF_SIZ_INT];		// price
	int				adk[DBF_SIZ_INT];		// apply_day_kind
	int				mei[DBF_SIZ_INT];		// modified_employee_id
	Day				cdt[DBF_SIZ_DTM];		// created_date(datetime)
	Day				mdt[DBF_SIZ_DTM];		// modified_date(datetime)
	Day				ddt[DBF_SIZ_DTM];		// deleted_date(datetime)
}EQP_CGD;

typedef	struct	{							// equipment_states segment
	int				id[DBF_SIZ_INT];		// id
	WCHAR			opc[DBF_SIZ_WST(30)];	// operator_code
	int				eii[DBF_SIZ_INT];		// equipment_info_id
	int				sen[DBF_SIZ_INT];		// sale_enable
	int				cln[DBF_SIZ_INT];		// clean_state
	int				ocp[DBF_SIZ_INT];		// occupied_state
	int				mei[DBF_SIZ_INT];		// modified_employee_id
	Day				cdt[DBF_SIZ_DTM];		// created_date
	Day				mdt[DBF_SIZ_DTM];		// modified_date
	Day				ddt[DBF_SIZ_DTM];		// deleted_date
}EQP_STS;

typedef	struct	{							// sales_infos segment
	int				id[DBF_SIZ_INT];		// id
	WCHAR			opc[DBF_SIZ_WST(30)];	// operator_code
	int				rii[DBF_SIZ_INT];		// reservation_info_id
	int				rdi[DBF_SIZ_INT];		// reservation_detail_id
	int				eii[DBF_SIZ_INT];		// equipment_info_id
	WCHAR			brc[DBF_SIZ_WST(30)];	// branch_code
	WCHAR			fcc[DBF_SIZ_WST(30)];	// facility_code
	int				ivn[DBF_SIZ_INT];		// invoice_number
	WCHAR			flg[DBF_SIZ_WST(30)];	// folio_language
	int				pid[DBF_SIZ_INT];		// paid
	int				mei[DBF_SIZ_INT];		// modified_employee_id
	Day				cdt[DBF_SIZ_DTM];		// created_date
	Day				mdt[DBF_SIZ_DTM];		// modified_date
	Day				ddt[DBF_SIZ_DTM];		// deleted_date
}SAL_INF;


typedef	struct	{							// sales_details segment
	int				id[DBF_SIZ_INT];		// id
	WCHAR			opc[DBF_SIZ_WST(30)];	// operator_code
	int				sii[DBF_SIZ_INT];		// sales_info_id
	Day				sld[DBF_SIZ_INT];		// saled_date
	Day				sdt[DBF_SIZ_INT];		// sales_datetime
	int				dsi[DBF_SIZ_INT];		// description_id
	double			qty[DBF_SIZ_INT];		// quantity
	double			nmp[DBF_SIZ_INT];		// nominal_price
	double			slp[DBF_SIZ_INT];		// sales_price
	int				asi[DBF_SIZ_INT];		// accounting_subject_id
	int				eqi[DBF_SIZ_INT];		// equipment_info_id
	int				mei[DBF_SIZ_INT];		// modified_employee_id
	Day				cdt[DBF_SIZ_DTM];		// created_date
	Day				mdt[DBF_SIZ_DTM];		// modified_date
	Day				ddt[DBF_SIZ_DTM];		// deleted_date
}SAL_DTL;

typedef	struct	{							// payment_details segment
	int				id[DBF_SIZ_INT];		// id
	WCHAR			opc[DBF_SIZ_WST(30)];	// operator_code
	int				sii[DBF_SIZ_INT];		// sales_info_id
	int				sdi[DBF_SIZ_INT];		// sales_detail_id
	Day				pmd[DBF_SIZ_INT];		// payment_date
	Day				pdt[DBF_SIZ_INT];		// payment_datetime
	int				cci[DBF_SIZ_INT];		// currency_conversion_id
	double			rte[DBF_SIZ_INT];		// payment_exchange_rate
	double			amt[DBF_SIZ_INT];		// payment_amount
	int				mtd[DBF_SIZ_INT];		// payment_method
	int				dpk[DBF_SIZ_INT];		// deposit_kind
	int				mei[DBF_SIZ_INT];		// modified_employee_id
	Day				cdt[DBF_SIZ_DTM];		// created_date
	Day				mdt[DBF_SIZ_DTM];		// modified_date
	Day				ddt[DBF_SIZ_DTM];		// deleted_date
}PMT_DTL;

typedef	struct	{							// currency_conversuion base
	WCHAR			cod[256];				// currency code
	WCHAR			nam[256];				// currency name
	int				adp;					// after_decimal_point
	double			rte;					// exchange rate
}BSE_CRC;

typedef	struct	{							// Loggin information
	int				epi;					// employee_id
	WCHAR			ucc[4];					// use_country_code
}LGI_INF;

typedef	struct	{							// HotelPMS management segment
	SYS_MNG			*sys;					// Standard flamework management segment
	Lnk				brl[1];					// List of branch_infos
	Lnk				fcl[1];					// List of facility_infos
	Lnk				eil[1];					// List of equipnment_infos
	Lnk				etl[1];					// List of equipnment_types
	Lnk				dtl[1];					// List of duties
	Lnk				msl[1];					// List of market_segments
	Lnk				agl[1];					// List of agent_infos
	Lnk				bcl[1];					// List of business_calendar
	Lnk				ccl[1];					// List of international country information
	Lnk				lgl[1];					// List of international language information
	WCHAR			opc[31];				// operator_code
	WCHAR			brc[31];				// branch code in use
	WCHAR			fcc[31];				// facility code in use
	BSE_CRC			bsc[1];					// based currency conversion
	Day				bdt[1];					// business date
	EMP_LYE			lgi[1];					// loggin employee information
//	int				lie;					// logged in employee id
	WCHAR			dfl[20];				// default_language(Instead when system_languageof client_infos is null)
	OPR_INF			opr[1];					// data of current oprrator_infos 
	BRC_INF			*bri;					// pointer of current branch_infos
	FCL_INF			*fci;					// pointer of current facility_infos
}PMS_MNG;

#endif
