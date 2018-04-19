/*
	ホテルプロパティ管理システム
	検索ウィンドウ用データベースアクセス関数
*/

#include <afx.h>
#include <afxdb.h>
#include "..\..\Common\bsc_def.h"
#include "..\..\Common\sys_def.h"
#include "..\..\Common\dbs_mng.h"
#include "..\..\Common\cmn_fnc.h"
#include "..\..\Common\sys_log.h"

#include "..\..\pms_def.h"
#include "fld_def.h"
#include "sql_cmn.h"
#include "rsv_fnc.h"
#include "rsv_sql.h"

static	WCHAR	*opr_inf[] = {
	{ _T("id")							},
	{ _T("operator_code")				},
	{ _T("name")						},
	{ _T("abbreviation")				},
	{ _T("address1")					},
	{ _T("address2")					},
	{ _T("telephone_number")			},
	{ _T("fax_number")					},
	{ _T("web_url")						},
	{ _T("email_address")				},
	{ _T("logo_image_path")				},
	{ _T("CM_user_token")				},
	{ _T("accounting_subject_kind")		},
	{ _T("accounting_method")			},
	{ _T("accounting_department_id")	},
	{ _T("modified_employee_id")		},
	{ _T("created_date")				},
	{ _T("modified_date")				},
	{ _T("deleted_date")				}
};

static	WCHAR	*brc_inf[] = {
	{ _T("id")						},
	{ _T("operator_code")			},
	{ _T("branch_code")				},
	{ _T("name")					},
	{ _T("abbreviation")			},
	{ _T("address1")				},
	{ _T("address2")				},
	{ _T("telephone_number")		},
	{ _T("fax_number")				},
	{ _T("web_url")					},
	{ _T("email_address")			},
	{ _T("company_store")			},
	{ _T("logo_image_path")			},
	{ _T("CM_property_id")			},
	{ _T("accounting_subject_kind")	},
	{ _T("accounting_method")		},
	{ _T("accounting_department_id")},
	{ _T("modified_employee_id")	},
	{ _T("created_date")			},
	{ _T("modified_date")			},
	{ _T("deleted_date")			}
};

static	WCHAR	*fcl_inf[] = {
	{ _T("id")							},
	{ _T("operator_code")				},
	{ _T("branch_code")					},
	{ _T("facility_code")				},
	{ _T("name")						},
	{ _T("abbreviation")				},
	{ _T("have_equipments")				},
	{ _T("normal_time_check_in")		},
	{ _T("normal_time_check_out")		},
	{ _T("exemption_early_check_in")	},
	{ _T("exemption_late_check_out")	},
	{ _T("address1")					},
	{ _T("address2")					},
	{ _T("telephone_number")			},
	{ _T("fax_number")					},
	{ _T("web_url")						},
	{ _T("email_address")				},
	{ _T("company_store")				},
	{ _T("facility_kind")				},
	{ _T("facility_color")				},
	{ _T("optional_configuration")		},
	{ _T("day_close_charge")			},
	{ _T("accounting_department_id")	},
	{ _T("modified_employee_id")		},
	{ _T("created_date")				},
	{ _T("modified_date")				},
	{ _T("deleted_date")				}
};

static	WCHAR	*bld_inf[] = {
	{ _T("id")						},
	{ _T("operator_code")			},
	{ _T("branch_info_id")			},
	{ _T("facility_info_id")		},
	{ _T("building_name")			},
	{ _T("abbreviation")			},
	{ _T("modified_employee_id")	},
	{ _T("created_date")			},
	{ _T("modified_date")			},
	{ _T("deleted_date")			}
};

static	WCHAR	*dty_inf[] = {
	{ _T("id")						},
	{ _T("operator_code")			},
	{ _T("branch_code")				},
	{ _T("facility_code")			},
	{ _T("name")					},
	{ _T("abbreviation")			},
	{ _T("display_order")			},
	{ _T("modified_employee_id")	},
	{ _T("created_date")			},
	{ _T("modified_date")			},
	{ _T("deleted_date")			}
};

static	WCHAR	*emp_lye[] = {
	{ _T("id")						},
	{ _T("operator_code")			},
	{ _T("user_id")					},
	{ _T("employee_code")			},
	{ _T("branch_code")				},
	{ _T("facility_code")			},
	{ _T("first_name")				},
	{ _T("last_name")				},
	{ _T("gender")					},
	{ _T("date_of_birth")			},
	{ _T("married")					},
	{ _T("address1")				},
	{ _T("address2")				},
	{ _T("zip_code")				},
	{ _T("telephone_number1")		},
	{ _T("telephone_number2")		},
	{ _T("email_address1")			},
	{ _T("email_address2")			},
	{ _T("nationality")				},
	{ _T("language")				},
	{ _T("system_language")			},
	{ _T("id_number")				},
	{ _T("date_of_issue_of_id")		},
	{ _T("expiration_date_of_id")	},
	{ _T("employment_date")			},
	{ _T("role_id")					},
	{ _T("duties_id")				},
	{ _T("employee_kind")			},
	{ _T("photo_image_path")		},
	{ _T("password")				},
	{ _T("locked")					},
	{ _T("modified_employee_id")	},
	{ _T("created_date")			},
	{ _T("modified_date")			},
	{ _T("deleted_date")			}
};


static	WCHAR	*biz_cdr[] = {
	{ _T("id")						},
	{ _T("operator_code")			},
	{ _T("branch_code")				},
	{ _T("facility_code")			},
	{ _T("date_of_calendar")		},
	{ _T("calendar_kind")			},
	{ _T("comment")					},
	{ _T("modified_employee_id")	},
	{ _T("created_date")			},
	{ _T("modified_date")			},
	{ _T("deleted_date")			}
};

static	WCHAR	*eqp_inf[] = {
	{ _T("id")							},
	{ _T("operator_code")				},
	{ _T("branch_code")					},
	{ _T("facility_code")				},
	{ _T("equipment_kind")				},
	{ _T("equipment_type_id")			},
	{ _T("facility_building_info_id")	},
	{ _T("equipment_code")				},
	{ _T("additional_info1")			},
	{ _T("additional_info2")			},
	{ _T("additional_info3")			},
	{ _T("name")						},
	{ _T("abbreviation")				},
	{ _T("regular_capacity")			},
	{ _T("extra_capacity")				},
	{ _T("sale_enable")					},
	{ _T("employee_id")					},
	{ _T("modified_employee_id")		},
	{ _T("created_date")				},
	{ _T("modified_date")				},
	{ _T("deleted_date")				}
};

static	WCHAR	*eqp_typ[] = {
	{ _T("id")							},
	{ _T("operator_code")				},
	{ _T("branch_code")					},
	{ _T("facility_code")				},
	{ _T("equipment_kind")				},
	{ _T("name")						},
	{ _T("abbreviation")				},
	{ _T("CM_occupancy")				},
	{ _T("CM_room_myallocator_id")		},
	{ _T("CM_unit")						},
	{ _T("modified_employee_id")		},
	{ _T("created_date")				},
	{ _T("modified_date")				},
	{ _T("deleted_date")				}
};

static	WCHAR	*eqp_sts[] = {
	{ _T("id")							},
	{ _T("operator_code")				},
	{ _T("equipment_info_id")			},
	{ _T("sale_enable")					},
	{ _T("clean_state")					},
	{ _T("occupied_state")				},
	{ _T("modified_employee_id")		},
	{ _T("created_date")				},
	{ _T("modified_date")				},
	{ _T("deleted_date")				}
};

static	WCHAR	*mkt_seg[] = {
	{ _T("id")							},
	{ _T("operator_code")				},
	{ _T("branch_code")					},
	{ _T("facility_code")				},
	{ _T("name")						},
	{ _T("abbreviation")				},
	{ _T("comment")						},
	{ _T("modified_employee_id")		},
	{ _T("created_date")				},
	{ _T("modified_date")				},
	{ _T("deleted_date")				}
};

static	WCHAR	*agt_inf[] = {
	{ _T("id")							},
	{ _T("operator_code")				},
	{ _T("agent_code")					},
	{ _T("name")						},
	{ _T("abbreviation")				},
	{ _T("agent_kind")					},
	{ _T("address1")					},
	{ _T("address2")					},
	{ _T("zip_code")					},
	{ _T("telephone_number1")			},
	{ _T("telephone_number2")			},
	{ _T("fax_number")					},
	{ _T("email_address1")				},
	{ _T("email_address2")				},
	{ _T("url")							},
	{ _T("country")						},
	{ _T("reservation_count")			},
	{ _T("establishment_count")			},
	{ _T("cancel_count")				},
	{ _T("CM_ota_channel_code")			},
	{ _T("modified_employee_id")		},
	{ _T("created_date")				},
	{ _T("modified_date")				},
	{ _T("deleted_date")				}
};

static	WCHAR	*cdc_inf[] = {
	{ _T("id")							},
	{ _T("operator_code")				},
	{ _T("company_code")				},
	{ _T("name")						},
	{ _T("address1")					},
	{ _T("address2")					},
	{ _T("zip_code")					},
	{ _T("telephone_number1")			},
	{ _T("telephone_number2")			},
	{ _T("fax_number")					},
	{ _T("email_address1")				},
	{ _T("email_address2")				},
	{ _T("rate")						},
	{ _T("url")							},
	{ _T("country")						},
	{ _T("payment_day")					},
	{ _T("closing_day")					},
	{ _T("modified_employee_id")		},
	{ _T("created_date")				},
	{ _T("modified_date")				},
	{ _T("deleted_date")				}
};


static	WCHAR	*crc_cvs[] = {
	{ _T("id")						},
	{ _T("operator_code")			},
	{ _T("branch_code")				},
	{ _T("facility_code")			},
	{ _T("currency_code")			},
	{ _T("name")					},
	{ _T("is_base")					},
	{ _T("is_secondly")				},
	{ _T("after_dec_point")			},
	{ _T("exchange_rate")			},
	{ _T("modified_employee_id")	},
	{ _T("created_date")			},
	{ _T("modified_date")			},
	{ _T("deleted_date")			}
};

static	WCHAR	*rsv_inf[] = {
	{ _T("id")							},
	{ _T("operator_code")				},
	{ _T("branch_code")					},
	{ _T("facility_code")				},
	{ _T("reservation_number")			},
	{ _T("client_info_id")				},
	{ _T("representative_name")			},
	{ _T("representative_gender")		},
	{ _T("agent_info_id")				},
	{ _T("online_reservation")			},
	{ _T("external_reservation_number")	},
	{ _T("first_arrival_date")			},
	{ _T("last_departure_date")			},
	{ _T("canceled")					},
	{ _T("no_show")						},
	{ _T("check_in_status")				},
	{ _T("path_of_reservation")			},
	{ _T("note")						},
	{ _T("reservation_create_date")		},
	{ _T("market_segment_id")			},
	{ _T("payment_method")				},
	{ _T("modified_employee_id")		},
	{ _T("created_date")				},
	{ _T("modified_date")				},
	{ _T("deleted_date")				}
};


static	WCHAR	*rsv_dtl[] = {
	{ _T("id")						},
	{ _T("operator_code")			},
	{ _T("reservation_info_id")		},
	{ _T("client_info_id")			},
	{ _T("arrival_date")			},
	{ _T("arrival_time")			},
	{ _T("departure_date")			},
	{ _T("departure_time")			},
	{ _T("check_in_date")			},
	{ _T("check_out_date")			},
	{ _T("early_check_in")			},
	{ _T("late_check_out")			},
	{ _T("person_number")			},
	{ _T("children_number")			},
	{ _T("equipment_type_id")		},
	{ _T("inhouse")					},
	{ _T("canceled")				},
	{ _T("note")					},
	{ _T("modified_employee_id")	},
	{ _T("created_date")			},
	{ _T("modified_date")			},
	{ _T("deleted_date")			}
};

static	WCHAR	*cli_inf[] = {
	{ _T("id")						},
	{ _T("operator_code")			},
	{ _T("client_kind")				},
	{ _T("first_name")				},
	{ _T("last_name")				},
	{ _T("gender")					},
	{ _T("date_of_birth")			},
	{ _T("address1")				},
	{ _T("address2")				},
	{ _T("zip_code")				},
	{ _T("telephone_number1")		},
	{ _T("telephone_number2")		},
	{ _T("fax_number")				},
	{ _T("email_address1")			},
	{ _T("email_address2")			},
	{ _T("nationality")				},
	{ _T("language")				},
	{ _T("system_language")			},
	{ _T("currency_conversion_id")	},
	{ _T("id_number")				},
	{ _T("date_of_issue_of_id")		},
	{ _T("expiration_date_of_id")	},
	{ _T("visa_number")				},
	{ _T("expiration_date_of_visa")	},
	{ _T("client_rank")				},
	{ _T("married")					},
	{ _T("favorite_equipment1")		},
	{ _T("favorite_equipment2")		},
	{ _T("usage_count")				},
	{ _T("noshow_count")			},
	{ _T("photo_image_path")		},
	{ _T("is_smoker")				},
	{ _T("member_id")				},
	{ _T("tax_code")				},
	{ _T("creditcard_company_id")	},
	{ _T("creditcard_number")		},
	{ _T("creditcard_title")		},
	{ _T("modified_employee_id")	},
	{ _T("created_date")			},
	{ _T("modified_date")			},
	{ _T("deleted_date")			}
};

static	WCHAR	*rsv_eqp[] = {
	{ _T("id")						},
	{ _T("operator_code")			},
	{ _T("reservation_info_id")		},
	{ _T("reservation_detail_id")	},
	{ _T("equipment_type_id")		},
	{ _T("equipment_info_id")		},
	{ _T("use_start_date")			},
	{ _T("use_end_date")			},
	{ _T("inhouse")					},
	{ _T("last_used_date")			},
	{ _T("description_id")			},
	{ _T("actual_amount")			},
	{ _T("modified_employee_id")	},
	{ _T("created_date")			},
	{ _T("modified_date")			},
	{ _T("deleted_date")			}
};

static	WCHAR	*rsv_crg[] = {
	{ _T("id")							},
	{ _T("operator_code")				},
	{ _T("reservation_info_id")			},
	{ _T("reservation_detail_id")		},
	{ _T("reservation_equipment_id")	},
	{ _T("use_date")					},
	{ _T("charge_kind")					},
	{ _T("description_id")				},
	{ _T("equipment_charge_detail_id")	},
	{ _T("actual_amount")				},
	{ _T("paid_up")						},
	{ _T("modified_employee_id")		},
	{ _T("created_date")				},
	{ _T("modified_date")				},
	{ _T("deleted_date")				}
};

static	WCHAR	*dsc_rpt[] = {
	{ _T("id")							},
	{ _T("operator_code")				},
	{ _T("branch_code")					},
	{ _T("facility_code")				},
	{ _T("description_code")			},
	{ _T("description_kind")			},
	{ _T("description_category_id")		},
	{ _T("name")						},
	{ _T("abbreviation")				},
	{ _T("valid_sale_period_start")		},
	{ _T("valid_sale_period_end")		},
	{ _T("price_kind")					},
	{ _T("price")						},
	{ _T("tax_application")				},
	{ _T("tax_attribution")				},
	{ _T("sale_enable")					},
	{ _T("readable_code")				},
	{ _T("transfer_origin_facility_id")	},
	{ _T("modified_employee_id")		},
	{ _T("created_date")				},
	{ _T("modified_date")				},
	{ _T("deleted_date")				}
};


static	WCHAR	*dsc_cat[] = {
	{ _T("id")						},
	{ _T("operator_code")			},
	{ _T("branch_code")				},
	{ _T("facility_code")			},
	{ _T("name")					},
	{ _T("abbreviation")			},
	{ _T("display_order")			},
	{ _T("modified_employee_id")	},
	{ _T("created_date")			},
	{ _T("modified_date")			},
	{ _T("deleted_date")			}
};

static	WCHAR	*eqp_crg[] = {
	{ _T("id")						},
	{ _T("operator_code")			},
	{ _T("branch_code")				},
	{ _T("facility_code")			},
	{ _T("equipment_type_id")		},
	{ _T("equipment_info_id")		},
	{ _T("agent_info_id")			},
	{ _T("description_id")			},
	{ _T("charge_kind")				},
	{ _T("charge_particulars")		},
	{ _T("reservation_route")		},
	{ _T("persons")					},
	{ _T("CM_rooms_unit")			},
	{ _T("CM_minstay")				},
	{ _T("CM_maxstay")				},
	{ _T("modified_employee_id")	},
	{ _T("created_date")			},
	{ _T("modified_date")			},
	{ _T("deleted_date")			}
};

static	WCHAR	*eqp_cgd[] = {
	{ _T("id")						},
	{ _T("operator_code")			},
	{ _T("branch_code")				},
	{ _T("facility_code")			},
	{ _T("equipment_charge_info_id")},
	{ _T("price")					},
	{ _T("apply_day_kind")			},
	{ _T("modified_employee_id")	},
	{ _T("created_date")			},
	{ _T("modified_date")			},
	{ _T("deleted_date")			}
};

static	WCHAR	*sal_inf[] = {
	{ _T("id")						},
	{ _T("operator_code")			},
	{ _T("reservation_info_id")		},
	{ _T("reservation_detail_id")	},
	{ _T("equipment_info_id")		},
	{ _T("branch_code")				},
	{ _T("facility_code")			},
	{ _T("invoice_number")			},
	{ _T("folio_language")			},
	{ _T("paid")					},
	{ _T("modified_employee_id")	},
	{ _T("created_date")			},
	{ _T("modified_date")			},
	{ _T("deleted_date")			}
};

static	WCHAR	*sal_dtl[] = {
	{ _T("id")						},
	{ _T("operator_code")			},
	{ _T("sales_info_id")			},
	{ _T("sales_date")				},
	{ _T("sales_datetime")			},
	{ _T("description_id")			},
	{ _T("quantity")				},
	{ _T("nominal_price")			},
	{ _T("sales_price")				},
	{ _T("accounting_subject_id")	},
	{ _T("equipment_info_id")		},
	{ _T("modified_employee_id")	},
	{ _T("created_date")			},
	{ _T("modified_date")			},
	{ _T("deleted_date")			}
};

static	WCHAR	*pmt_dtl[] = {
	{ _T("id")						},
	{ _T("operator_code")			},
	{ _T("sales_info_id")			},
	{ _T("sales_detail_id")			},
	{ _T("payment_date")			},
	{ _T("payment_datetime")		},
	{ _T("currency_conversion_id")	},
	{ _T("payment_exchange_rate")	},
	{ _T("payment_amount")			},
	{ _T("payment_method")			},
	{ _T("deposit_kind")			},
	{ _T("modified_employee_id")	},
	{ _T("created_date")			},
	{ _T("modified_date")			},
	{ _T("deleted_date")			}
};


static	int		get_tdf(		// Getting table definition
WCHAR			***ptr,			// Destination
int				knd )			// table kind
{
	int			cnt = 0;

	switch( knd ){
	case FDF_OPR_INF:	*ptr = opr_inf;	cnt=memcnt(opr_inf);	break;	
	case FDF_BRC_INF:	*ptr = brc_inf;	cnt=memcnt(brc_inf);	break;	
	case FDF_FCL_INF:	*ptr = fcl_inf;	cnt=memcnt(fcl_inf);	break;	
	case FDF_DTY_INF:	*ptr = dty_inf;	cnt=memcnt(dty_inf);	break;
	case FDF_EMP_LYE:	*ptr = emp_lye;	cnt=memcnt(emp_lye);	break;
	case FDF_BLD_INF:	*ptr = bld_inf;	cnt=memcnt(bld_inf);	break;
	case FDF_BIZ_CDR:	*ptr = biz_cdr;	cnt=memcnt(biz_cdr);	break;	
	case FDF_EQP_INF:	*ptr = eqp_inf;	cnt=memcnt(eqp_inf);	break;	
	case FDF_EQP_TYP:	*ptr = eqp_typ;	cnt=memcnt(eqp_typ);	break;	
	case FDF_EQP_STS:	*ptr = eqp_sts;	cnt=memcnt(eqp_sts);	break;	
	case FDF_MKT_SEG:	*ptr = mkt_seg;	cnt=memcnt(mkt_seg);	break;	
	case FDF_AGT_INF:	*ptr = agt_inf;	cnt=memcnt(agt_inf);	break;	
	case FDF_CDC_INF:	*ptr = cdc_inf;	cnt=memcnt(cdc_inf);	break;	
	case FDF_CLI_INF:	*ptr = cli_inf;	cnt=memcnt(cli_inf);	break;	
	case FDF_CRC_CVS:	*ptr = crc_cvs;	cnt=memcnt(crc_cvs);	break;	
	case FDF_RSV_INF:	*ptr = rsv_inf;	cnt=memcnt(rsv_inf);	break;	
	case FDF_RSV_DTL:	*ptr = rsv_dtl;	cnt=memcnt(rsv_dtl);	break;	
	case FDF_RSV_EQP:	*ptr = rsv_eqp;	cnt=memcnt(rsv_eqp);	break;	
	case FDF_RSV_CRG:	*ptr = rsv_crg;	cnt=memcnt(rsv_crg);	break;
	case FDF_DSC_RPT:	*ptr = dsc_rpt;	cnt=memcnt(dsc_rpt);	break;
	case FDF_DSC_CAT:	*ptr = dsc_cat;	cnt=memcnt(dsc_cat);	break;
	case FDF_EQP_CRG:	*ptr = eqp_crg;	cnt=memcnt(eqp_crg);	break;
	case FDF_EQP_CGD:	*ptr = eqp_cgd;	cnt=memcnt(eqp_cgd);	break;
	case FDF_SAL_INF:	*ptr = sal_inf;	cnt=memcnt(sal_inf);	break;
	case FDF_SAL_DTL:	*ptr = sal_dtl;	cnt=memcnt(sal_dtl);	break;
	case FDF_PMT_DTL:	*ptr = pmt_dtl;	cnt=memcnt(pmt_dtl);	break;
	default:			*ptr = NULL;							break;
	}

	return( cnt );
}

static	int		mkf_cpy(		// Making database field definition copy function
Dbs_fld			**dst,			// Destination pointer
Dbs_fld			*src,			// source pointer
int				sct,			// Source fields number
WCHAR			*pfx,			// Prefix of each items
int				knd,			// table kind
Dbs_fld			*ext,			// Extended fields definition
int				ect )			// Extended fields number
{
	int			pos, idx, cnt, siz=0, its, psz=!!pfx ? (int)wcslen(pfx): 0;
	WCHAR		**ptr=NULL, *dsp;

	cnt = get_tdf( &ptr, knd );
	if ( ptr ){
		for ( idx=0; idx<cnt; idx++ ) siz += psz+(int)wcslen( *(ptr+idx) ) + 2;
	}

	*dst = (Dbs_fld *)malloc( sizeof(Dbs_fld)*sct + ( !!ext ? sizeof(Dbs_fld)*ect: 0 ) + wsizeof(siz) );
	memcpy( *dst, src, sizeof(Dbs_fld)*sct );
	if ( !!ext ){
		memcpy( (*dst)+sct, ext, sizeof(Dbs_fld)*ect );
	}

	dsp = (WCHAR *)((*dst) + sct + ( !!ext ? ect: 0 ) );
	for ( idx=pos=0; idx<cnt; idx++ ){
		((*dst)+idx)->fln = dsp;
		pos += ( its = swprintf_s( dsp, siz-pos, _T("%s%s"), !!pfx ? pfx: _T(""), *(ptr+idx) ) + 1 );
		dsp += its;
	}

	return( !!(*dst) ? sct+(!!ext ? ect:0) : 0 );
}

int			Fdf_Mak_Sll(		// Make select list
WCHAR		**dst,				// destination pointer
int			knd,				// Kind of table
WCHAR		*ars,				// areas name of table
WCHAR		*pfx )				// Prefix of each item name
{
	WCHAR		**ptr=NULL, *dsp;
	int			idx, cnt, siz=0, pos, asz=(!!ars ? (int)wcslen(ars): 0 ), psz=!!pfx ? (int)wcslen(pfx): 0;

	*dst = NULL;
	cnt = get_tdf( &ptr, knd );
	if ( ptr ){
		if ( !!ars ){
			for ( idx=0; idx<cnt; idx++ ) siz += ( psz + asz + (int)wcslen( *(ptr+idx) ) ) * 2 + 12;
			if ( *dst = dsp = (WCHAR *)malloc( wsizeof(siz) ) ){
				for ( idx=pos=0; idx<cnt; idx++ ){
					pos += swprintf_s( *dst+pos, siz-pos, _T("%s%s.%s AS %s%s"),
						!idx ? _T(""): _T(","), ars, *(ptr+idx), !!pfx ? pfx: _T(""), *(ptr+idx) );
				}
			}
		}
		else{
			for ( idx=0; idx<cnt; idx++ ) siz += psz+(int)wcslen( *(ptr+idx) ) + 3;
			if ( *dst = dsp = (WCHAR *)malloc( wsizeof(siz) ) ){
				for ( idx=pos=0; idx<cnt; idx++ ){
					pos += swprintf_s( *dst+pos, siz-pos, _T("%s%s%s"),
						!idx ? _T(""): _T(","), !!pfx ? pfx: _T(""), *(ptr+idx) );
				}
			}
		}
	}

	return( *dst ? siz: 0 );
}

int			Fdf_Opr_Inf(		// Making the opetator_infos field definition
Dbs_fld			**dst,			// Destination pointer
void			*ptr,			// pointer of target structure
WCHAR			*pfx,			// prefix of each items
Dbs_fld			*ext,			// Extended fields definition
int				ect )			// Extended fields number
{
	OPR_INF		*opr=(OPR_INF *)ptr;
	Dbs_fld		fld[] = {	{ opr->id,	NULL, FLD_INT, FLD_INT_SIZ(opr->id) },
							{ opr->cod, NULL, FLD_WST, FLD_WST_SIZ(opr->cod) },
							{ opr->nam, NULL, FLD_WST, FLD_WST_SIZ(opr->nam) },
							{ opr->abr, NULL, FLD_WST, FLD_WST_SIZ(opr->abr) },
							{ opr->adr1,NULL, FLD_WST, FLD_WST_SIZ(opr->adr1) },
							{ opr->adr2,NULL, FLD_WST, FLD_WST_SIZ(opr->adr2) },
							{ opr->tel, NULL, FLD_WST, FLD_WST_SIZ(opr->tel) },
							{ opr->fax, NULL, FLD_WST, FLD_WST_SIZ(opr->fax) },
							{ opr->url, NULL, FLD_WST, FLD_WST_SIZ(opr->url) },
							{ opr->eml, NULL, FLD_WST, FLD_WST_SIZ(opr->eml) },
							{ opr->lip, NULL, FLD_WST, FLD_WST_SIZ(opr->lip) },
							{ opr->tkn, NULL, FLD_WST, FLD_WST_SIZ(opr->tkn) },
							{ opr->asb, NULL, FLD_INT, FLD_INT_SIZ(opr->asb) },
							{ opr->acm, NULL, FLD_INT, FLD_INT_SIZ(opr->acm) },
							{ opr->adi, NULL, FLD_INT, FLD_INT_SIZ(opr->adi) },
							{ opr->mei, NULL, FLD_INT, FLD_INT_SIZ(opr->mei) },
							{ opr->cdt, NULL, FLD_DTM, FLD_DTM_SIZ(opr->cdt) },
							{ opr->mdt, NULL, FLD_DTM, FLD_DTM_SIZ(opr->mdt) },
							{ opr->ddt, NULL, FLD_DTM, FLD_DTM_SIZ(opr->ddt) } };

	return( mkf_cpy( dst, fld, memcnt(fld), pfx, FDF_OPR_INF, ext, ect ) );
}

int			Fdf_Brc_Inf(		// Making the branch_infos field definition
Dbs_fld			**dst,			// Destination pointer
void			*ptr,			// pointer of target structure
WCHAR			*pfx,			// prefix of each items
Dbs_fld			*ext,			// Extended fields definition
int				ect )			// Extended fields number
{
	BRC_INF		*brc = (BRC_INF *)ptr;
	Dbs_fld		fld[] = {	{ brc->id,	NULL, FLD_INT, FLD_INT_SIZ(brc->id), },
							{ brc->opc,	NULL, FLD_WST, FLD_WST_SIZ(brc->opc) },
							{ brc->brc,	NULL, FLD_WST, FLD_WST_SIZ(brc->brc) },
							{ brc->nam,	NULL, FLD_WST, FLD_WST_SIZ(brc->nam) },
							{ brc->abr,	NULL, FLD_WST, FLD_WST_SIZ(brc->abr) },
							{ brc->adr1,NULL, FLD_WST, FLD_WST_SIZ(brc->adr1) },
							{ brc->adr2,NULL, FLD_WST, FLD_WST_SIZ(brc->adr2) },
							{ brc->tel,	NULL, FLD_WST, FLD_WST_SIZ(brc->tel) },
							{ brc->fax,	NULL, FLD_WST, FLD_WST_SIZ(brc->fax) },
							{ brc->url,	NULL, FLD_WST, FLD_WST_SIZ(brc->url) },
							{ brc->eml,	NULL, FLD_WST, FLD_WST_SIZ(brc->eml) },
							{ brc->cms,	NULL, FLD_INT, FLD_INT_SIZ(brc->cms) },
							{ brc->lgp,	NULL, FLD_WST, FLD_WST_SIZ(brc->lgp) },
							{ brc->cpi,	NULL, FLD_INT, FLD_INT_SIZ(brc->cpi) },
							{ brc->ask,	NULL, FLD_INT, FLD_INT_SIZ(brc->ask) },
							{ brc->acm,	NULL, FLD_INT, FLD_INT_SIZ(brc->acm) },
							{ brc->adi,	NULL, FLD_INT, FLD_INT_SIZ(brc->adi) },
							{ brc->mei,	NULL, FLD_INT, FLD_INT_SIZ(brc->mei) },
							{ brc->cdt,	NULL, FLD_DTM, FLD_DTM_SIZ(brc->cdt) },
							{ brc->mdt,	NULL, FLD_DTM, FLD_DTM_SIZ(brc->mdt) },
							{ brc->ddt,	NULL, FLD_DTM, FLD_DTM_SIZ(brc->ddt) } };

	return( mkf_cpy( dst, fld, memcnt(fld), pfx, FDF_BRC_INF, ext, ect ) );
}

int			Fdf_Fcl_Inf(		// Making the facility_infos field definition
Dbs_fld			**dst,			// Destination pointer
void			*ptr,			// pointer of target structure
WCHAR			*pfx,			// prefix of each items
Dbs_fld			*ext,			// Extended fields definition
int				ect )			// Extended fields number
{
	FCL_INF		*fcc = (FCL_INF *)ptr;
	Dbs_fld		fld[] = {	{ fcc->id,	NULL, FLD_INT, FLD_INT_SIZ(fcc->id), },
							{ fcc->opc,	NULL, FLD_WST, FLD_WST_SIZ(fcc->opc) },
							{ fcc->brc,	NULL, FLD_WST, FLD_WST_SIZ(fcc->brc) },
							{ fcc->fcc,	NULL, FLD_WST, FLD_WST_SIZ(fcc->fcc) },
							{ fcc->nam,	NULL, FLD_WST, FLD_WST_SIZ(fcc->nam) },
							{ fcc->abr,	NULL, FLD_WST, FLD_WST_SIZ(fcc->abr) },
							{ fcc->hve,	NULL, FLD_INT, FLD_INT_SIZ(fcc->hve) },
							{ fcc->cit,	NULL, FLD_TIM, FLD_TIM_SIZ(fcc->cit) },
							{ fcc->cot,	NULL, FLD_TIM, FLD_TIM_SIZ(fcc->cot) },
							{ fcc->eci,	NULL, FLD_INT, FLD_INT_SIZ(fcc->eci) },
							{ fcc->lco,	NULL, FLD_INT, FLD_INT_SIZ(fcc->lco) },
							{ fcc->adr1,NULL, FLD_WST, FLD_WST_SIZ(fcc->adr1) },
							{ fcc->adr2,NULL, FLD_WST, FLD_WST_SIZ(fcc->adr2) },
							{ fcc->tel,	NULL, FLD_WST, FLD_WST_SIZ(fcc->tel) },
							{ fcc->fax,	NULL, FLD_WST, FLD_WST_SIZ(fcc->fax) },
							{ fcc->url,	NULL, FLD_WST, FLD_WST_SIZ(fcc->url) },
							{ fcc->eml,	NULL, FLD_WST, FLD_WST_SIZ(fcc->eml) },
							{ fcc->cms,	NULL, FLD_INT, FLD_INT_SIZ(fcc->cms) },
							{ fcc->fck,	NULL, FLD_INT, FLD_INT_SIZ(fcc->fck) },
							{ fcc->fcl,	NULL, FLD_INT, FLD_INT_SIZ(fcc->fcl) },
							{ fcc->opt,	NULL, FLD_INT, FLD_INT_SIZ(fcc->opt) },
							{ fcc->dcc,	NULL, FLD_INT, FLD_INT_SIZ(fcc->dcc) },
							{ fcc->adi,	NULL, FLD_INT, FLD_INT_SIZ(fcc->adi) },
							{ fcc->mei,	NULL, FLD_INT, FLD_INT_SIZ(fcc->mei) },
							{ fcc->cdt,	NULL, FLD_DTM, FLD_DTM_SIZ(fcc->cdt) },
							{ fcc->mdt,	NULL, FLD_DTM, FLD_DTM_SIZ(fcc->mdt) },
							{ fcc->ddt,	NULL, FLD_DTM, FLD_DTM_SIZ(fcc->ddt) },  };

	return( mkf_cpy( dst, fld, memcnt(fld), pfx, FDF_FCL_INF, ext, ect ) );
}

int			Fdf_Bld_Inf(		// Making the facility_building_infos field definition
Dbs_fld			**dst,			// Destination pointer
void			*ptr,			// pointer of target structure
WCHAR			*pfx,			// prefix of each items
Dbs_fld			*ext,			// Extended fields definition
int				ect )			// Extended fields number
{
	BLD_INF		*bld = (BLD_INF *)ptr;
	Dbs_fld		fld[] = {	{ bld->id,	NULL, FLD_INT, FLD_INT_SIZ(bld->id), },
							{ bld->opc,	NULL, FLD_WST, FLD_WST_SIZ(bld->opc) },
							{ bld->brc,	NULL, FLD_WST, FLD_WST_SIZ(bld->brc) },
							{ bld->fcc,	NULL, FLD_WST, FLD_WST_SIZ(bld->fcc) },
							{ bld->nam,	NULL, FLD_WST, FLD_WST_SIZ(bld->nam) },
							{ bld->abr,	NULL, FLD_WST, FLD_WST_SIZ(bld->abr) },
							{ bld->mei,	NULL, FLD_INT, FLD_INT_SIZ(bld->mei) },
							{ bld->cdt,	NULL, FLD_DTM, FLD_DTM_SIZ(bld->cdt) },
							{ bld->mdt,	NULL, FLD_DTM, FLD_DTM_SIZ(bld->mdt) },
							{ bld->ddt,	NULL, FLD_DTM, FLD_DTM_SIZ(bld->ddt) },  };

	return( mkf_cpy( dst, fld, memcnt(fld), pfx, FDF_BLD_INF, ext, ect ) );
}

int			Fdf_Dty_Inf(		// Making the duties field definition
Dbs_fld			**dst,			// Destination pointer
void			*ptr,			// pointer of target structure
WCHAR			*pfx,			// prefix of each items
Dbs_fld			*ext,			// Extended fields definition
int				ect )			// Extended fields number
{
	DTY_INF		*dty = (DTY_INF *)ptr;
	Dbs_fld		fld[] = {	{ dty->id,	NULL, FLD_INT, FLD_INT_SIZ(dty->id), },
							{ dty->opc,	NULL, FLD_WST, FLD_WST_SIZ(dty->opc) },
							{ dty->brc,	NULL, FLD_WST, FLD_WST_SIZ(dty->brc) },
							{ dty->fcc,	NULL, FLD_WST, FLD_WST_SIZ(dty->fcc) },
							{ dty->nam,	NULL, FLD_WST, FLD_WST_SIZ(dty->nam) },
							{ dty->abr,	NULL, FLD_WST, FLD_WST_SIZ(dty->abr) },
							{ dty->odr,	NULL, FLD_INT, FLD_INT_SIZ(dty->odr) },
							{ dty->mei,	NULL, FLD_INT, FLD_INT_SIZ(dty->mei) },
							{ dty->cdt,	NULL, FLD_DTM, FLD_DTM_SIZ(dty->cdt) },
							{ dty->mdt,	NULL, FLD_DTM, FLD_DTM_SIZ(dty->mdt) },
							{ dty->ddt,	NULL, FLD_DTM, FLD_DTM_SIZ(dty->ddt) },  };

	return( mkf_cpy( dst, fld, memcnt(fld), pfx, FDF_DTY_INF, ext, ect ) );
}

int			Fdf_Emp_Lye(		// Making the employees field definition
Dbs_fld			**dst,			// Destination pointer
void			*ptr,			// pointer of target structure
WCHAR			*pfx,			// prefix of each items
Dbs_fld			*ext,			// Extended fields definition
int				ect )			// Extended fields number
{
	EMP_LYE		*epl = (EMP_LYE *)ptr;
	Dbs_fld		fld[] = {	{ epl->id,	NULL, FLD_INT, FLD_INT_SIZ(epl->id) },
							{ epl->opc,	NULL, FLD_WST, FLD_WST_SIZ(epl->opc) },
							{ epl->uii,	NULL, FLD_INT, FLD_INT_SIZ(epl->uii) },
							{ epl->epc,	NULL, FLD_WST, FLD_WST_SIZ(epl->epc) },
							{ epl->brc,	NULL, FLD_WST, FLD_WST_SIZ(epl->brc) },
							{ epl->fcc,	NULL, FLD_WST, FLD_WST_SIZ(epl->fcc) },
							{ epl->fnm,	NULL, FLD_WST, FLD_WST_SIZ(epl->fnm) },
							{ epl->lnm,	NULL, FLD_WST, FLD_WST_SIZ(epl->lnm) },
							{ epl->gdr,	NULL, FLD_INT, FLD_INT_SIZ(epl->gdr) },
							{ epl->bth,	NULL, FLD_DAY, FLD_DAY_SIZ(epl->bth) },
							{ epl->mrd,	NULL, FLD_INT, FLD_INT_SIZ(epl->mrd) },
							{ epl->adr1,NULL, FLD_WST, FLD_WST_SIZ(epl->adr1) },
							{ epl->adr2,NULL, FLD_WST, FLD_WST_SIZ(epl->adr2) },
							{ epl->zip,	NULL, FLD_WST, FLD_WST_SIZ(epl->zip) },
							{ epl->tel1,NULL, FLD_WST, FLD_WST_SIZ(epl->tel1) },
							{ epl->tel2,NULL, FLD_WST, FLD_WST_SIZ(epl->tel2) },
							{ epl->eml1,NULL, FLD_WST, FLD_WST_SIZ(epl->eml1) },
							{ epl->eml2,NULL, FLD_WST, FLD_WST_SIZ(epl->eml2) },
							{ epl->nto,	NULL, FLD_WST, FLD_WST_SIZ(epl->nto) },
							{ epl->lng,	NULL, FLD_WST, FLD_WST_SIZ(epl->lng) },
							{ epl->slg,	NULL, FLD_WST, FLD_WST_SIZ(epl->slg) },
							{ epl->idn,	NULL, FLD_WST, FLD_WST_SIZ(epl->idn) },
							{ epl->isd,	NULL, FLD_DAY, FLD_DAY_SIZ(epl->isd) },
							{ epl->exd,	NULL, FLD_DAY, FLD_DAY_SIZ(epl->exd) },
							{ epl->epd,	NULL, FLD_DAY, FLD_DAY_SIZ(epl->epd) },
							{ epl->rli,	NULL, FLD_INT, FLD_INT_SIZ(epl->rli) },
							{ epl->dti,	NULL, FLD_INT, FLD_INT_SIZ(epl->dti) },
							{ epl->knd,	NULL, FLD_INT, FLD_INT_SIZ(epl->knd) },
							{ epl->pth,	NULL, FLD_WST, FLD_WST_SIZ(epl->pth) },
							{ epl->pwd,	NULL, FLD_WST, FLD_WST_SIZ(epl->pwd) },
							{ epl->lck,	NULL, FLD_INT, FLD_INT_SIZ(epl->lck) },
							{ epl->mei,	NULL, FLD_INT, FLD_INT_SIZ(epl->mei) },
							{ epl->cdt,	NULL, FLD_DTM, FLD_DTM_SIZ(epl->cdt) },
							{ epl->mdt,	NULL, FLD_DTM, FLD_DTM_SIZ(epl->mdt) },
							{ epl->ddt,	NULL, FLD_DTM, FLD_DTM_SIZ(epl->ddt) }  };

	return( mkf_cpy( dst, fld, memcnt(fld), pfx, FDF_EMP_LYE, ext, ect ) );
}

int			Fdf_Biz_Cdr(		// Making the business_calendar field definition
Dbs_fld			**dst,			// Destination pointer
void			*ptr,			// pointer of target structure
WCHAR			*pfx,			// prefix of each items
Dbs_fld			*ext,			// Extended fields definition
int				ect )			// Extended fields number
{
	BIZ_CDR		*bzc = (BIZ_CDR *)ptr;
	Dbs_fld		fld[] = {	{ bzc->id,	NULL, FLD_INT, FLD_INT_SIZ(bzc->id), },
							{ bzc->opc,	NULL, FLD_WST, FLD_WST_SIZ(bzc->opc) },
							{ bzc->brc,	NULL, FLD_WST, FLD_WST_SIZ(bzc->brc) },
							{ bzc->fcc,	NULL, FLD_WST, FLD_WST_SIZ(bzc->fcc) },
							{ bzc->dte,	NULL, FLD_DAY, FLD_DAY_SIZ(bzc->dte) },
							{ bzc->knd,	NULL, FLD_INT, FLD_INT_SIZ(bzc->knd) },
							{ bzc->cmt,	NULL, FLD_WST, FLD_WST_SIZ(bzc->cmt) },
							{ bzc->mei,	NULL, FLD_INT, FLD_INT_SIZ(bzc->mei) },
							{ bzc->cdt,	NULL, FLD_DTM, FLD_DTM_SIZ(bzc->cdt) },
							{ bzc->mdt,	NULL, FLD_DTM, FLD_DTM_SIZ(bzc->mdt) },
							{ bzc->ddt,	NULL, FLD_DTM, FLD_DTM_SIZ(bzc->ddt) } };

	return( mkf_cpy( dst, fld, memcnt(fld), pfx, FDF_BIZ_CDR, ext, ect ) );
}

int			Fdf_Eqp_Inf(		// Making the equipment_infos field definition
Dbs_fld			**dst,			// Destination pointer
void			*ptr,			// pointer of target structure
WCHAR			*pfx,			// prefix of each items
Dbs_fld			*ext,			// Extended fields definition
int				ect )			// Extended fields number
{
	EQP_INF		*eqp = (EQP_INF *)ptr;
	Dbs_fld		fld[] = {	{ eqp->id,	NULL, FLD_INT, FLD_INT_SIZ(eqp->id), },
							{ eqp->opc,	NULL, FLD_WST, FLD_WST_SIZ(eqp->opc) },
							{ eqp->brc,	NULL, FLD_WST, FLD_WST_SIZ(eqp->brc) },
							{ eqp->fcc,	NULL, FLD_WST, FLD_WST_SIZ(eqp->fcc) },
							{ eqp->ekd,	NULL, FLD_INT, FLD_INT_SIZ(eqp->ekd) },
							{ eqp->eti,	NULL, FLD_INT, FLD_INT_SIZ(eqp->eti) },
							{ eqp->fbi,	NULL, FLD_INT, FLD_INT_SIZ(eqp->fbi) },
							{ eqp->eco,	NULL, FLD_WST, FLD_WST_SIZ(eqp->eco) },
							{ eqp->adi1,NULL, FLD_INT, FLD_INT_SIZ(eqp->adi1) },
							{ eqp->adi2,NULL, FLD_INT, FLD_INT_SIZ(eqp->adi2) },
							{ eqp->adi3,NULL, FLD_INT, FLD_INT_SIZ(eqp->adi3) },
							{ eqp->nam,	NULL, FLD_WST, FLD_WST_SIZ(eqp->nam) },
							{ eqp->abr,	NULL, FLD_WST, FLD_WST_SIZ(eqp->abr) },
							{ eqp->rcp,	NULL, FLD_INT, FLD_INT_SIZ(eqp->rcp) },
							{ eqp->ecp,	NULL, FLD_INT, FLD_INT_SIZ(eqp->ecp) },
							{ eqp->sle,	NULL, FLD_INT, FLD_INT_SIZ(eqp->sle) },
							{ eqp->epi,	NULL, FLD_INT, FLD_INT_SIZ(eqp->epi) },
							{ eqp->mei,	NULL, FLD_INT, FLD_INT_SIZ(eqp->mei) },
							{ eqp->cdt,	NULL, FLD_DTM, FLD_DTM_SIZ(eqp->cdt) },
							{ eqp->mdt,	NULL, FLD_DTM, FLD_DTM_SIZ(eqp->mdt) },
							{ eqp->ddt,	NULL, FLD_DTM, FLD_DTM_SIZ(eqp->ddt) } };

	return( mkf_cpy( dst, fld, memcnt(fld), pfx, FDF_EQP_INF, ext, ect ) );
}

int			Fdf_Mkt_Seg(		// Making the market_segment field definition
Dbs_fld			**dst,			// Destination pointer
void			*ptr,			// pointer of target structure
WCHAR			*pfx,			// prefix of each items
Dbs_fld			*ext,			// Extended fields definition
int				ect )			// Extended fields number
{
	MKT_SEG		*mks = (MKT_SEG *)ptr;
	Dbs_fld		fld[] = {	{ mks->id,	NULL, FLD_INT, FLD_INT_SIZ(mks->id)	 },
							{ mks->opc,	NULL, FLD_WST, FLD_WST_SIZ(mks->opc) },
							{ mks->brc,	NULL, FLD_WST, FLD_WST_SIZ(mks->brc) },
							{ mks->fcc,	NULL, FLD_WST, FLD_WST_SIZ(mks->fcc) },
							{ mks->nam,	NULL, FLD_WST, FLD_WST_SIZ(mks->nam) },
							{ mks->abr,	NULL, FLD_WST, FLD_WST_SIZ(mks->abr) },
							{ mks->cmt,	NULL, FLD_INT, FLD_INT_SIZ(mks->cmt) },
							{ mks->mei,	NULL, FLD_INT, FLD_INT_SIZ(mks->mei) },
							{ mks->cdt,	NULL, FLD_DTM, FLD_DTM_SIZ(mks->cdt) },
							{ mks->mdt,	NULL, FLD_DTM, FLD_DTM_SIZ(mks->mdt) },
							{ mks->ddt,	NULL, FLD_DTM, FLD_DTM_SIZ(mks->ddt) } };

	return( mkf_cpy( dst, fld, memcnt(fld), pfx, FDF_MKT_SEG, ext, ect ) );
}

int			Fdf_Eqp_Typ(		// Making the equipment_types field definition
Dbs_fld			**dst,			// Destination pointer
void			*ptr,			// pointer of target structure
WCHAR			*pfx,			// prefix of each items
Dbs_fld			*ext,			// Extended fields definition
int				ect )			// Extended fields number
{
	EQP_TYP		*eqt = (EQP_TYP *)ptr;
	Dbs_fld		fld[] = {	{ eqt->id,	NULL, FLD_INT, FLD_INT_SIZ(eqt->id), },
							{ eqt->opc,	NULL, FLD_WST, FLD_WST_SIZ(eqt->opc) },
							{ eqt->brc,	NULL, FLD_WST, FLD_WST_SIZ(eqt->brc) },
							{ eqt->fcc,	NULL, FLD_WST, FLD_WST_SIZ(eqt->fcc) },
							{ eqt->ekd,	NULL, FLD_INT, FLD_INT_SIZ(eqt->ekd) },
							{ eqt->nam,	NULL, FLD_WST, FLD_WST_SIZ(eqt->nam) },
							{ eqt->abr,	NULL, FLD_WST, FLD_WST_SIZ(eqt->abr) },
							{ eqt->cmo,	NULL, FLD_INT, FLD_INT_SIZ(eqt->cmo) },
							{ eqt->cmi,	NULL, FLD_INT, FLD_INT_SIZ(eqt->cmi) },
							{ eqt->cmu,	NULL, FLD_INT, FLD_INT_SIZ(eqt->cmu) },
							{ eqt->mei,	NULL, FLD_INT, FLD_INT_SIZ(eqt->mei) },
							{ eqt->cdt,	NULL, FLD_DTM, FLD_DTM_SIZ(eqt->cdt) },
							{ eqt->mdt,	NULL, FLD_DTM, FLD_DTM_SIZ(eqt->mdt) },
							{ eqt->ddt,	NULL, FLD_DTM, FLD_DTM_SIZ(eqt->ddt) } };

	return( mkf_cpy( dst, fld, memcnt(fld), pfx, FDF_EQP_TYP, ext, ect ) );
}

int			Fdf_Eqp_Sts(		// Making the equipment_states field definition
Dbs_fld			**dst,			// Destination pointer
void			*ptr,			// pointer of target structure
WCHAR			*pfx,			// prefix of each items
Dbs_fld			*ext,			// Extended fields definition
int				ect )			// Extended fields number
{
	EQP_STS		*sts = (EQP_STS *)ptr;
	Dbs_fld		fld[] = {	{ sts->id,	NULL, FLD_INT, FLD_INT_SIZ(sts->id), },
							{ sts->opc,	NULL, FLD_WST, FLD_WST_SIZ(sts->opc) },
							{ sts->eii,	NULL, FLD_INT, FLD_INT_SIZ(sts->eii) },
							{ sts->sen,	NULL, FLD_INT, FLD_INT_SIZ(sts->sen) },
							{ sts->cln,	NULL, FLD_INT, FLD_INT_SIZ(sts->cln) },
							{ sts->ocp,	NULL, FLD_INT, FLD_INT_SIZ(sts->ocp) },
							{ sts->mei,	NULL, FLD_INT, FLD_INT_SIZ(sts->mei) },
							{ sts->cdt,	NULL, FLD_DTM, FLD_DTM_SIZ(sts->cdt) },
							{ sts->mdt,	NULL, FLD_DTM, FLD_DTM_SIZ(sts->mdt) },
							{ sts->ddt,	NULL, FLD_DTM, FLD_DTM_SIZ(sts->ddt) } };

	return( mkf_cpy( dst, fld, memcnt(fld), pfx, FDF_EQP_STS, ext, ect ) );
}

int			Fdf_Agt_Inf(		// Making the agent_infos field definition
Dbs_fld			**dst,			// Destination pointer
void			*ptr,			// pointer of target structure
WCHAR			*pfx,			// prefix of each items
Dbs_fld			*ext,			// Extended fields definition
int				ect )			// Extended fields number
{
	AGT_INF		*agt = (AGT_INF *)ptr;
	Dbs_fld		fld[] = {	{ agt->id,	NULL, FLD_INT, FLD_INT_SIZ(agt->id), },
							{ agt->opc,	NULL, FLD_WST, FLD_WST_SIZ(agt->opc) },
							{ agt->agc,	NULL, FLD_WST, FLD_WST_SIZ(agt->agc) },
							{ agt->nam,	NULL, FLD_WST, FLD_WST_SIZ(agt->nam) },
							{ agt->abr,	NULL, FLD_WST, FLD_WST_SIZ(agt->abr) },
							{ agt->agk,	NULL, FLD_INT, FLD_INT_SIZ(agt->agk) },
							{ agt->adr1,NULL, FLD_WST, FLD_WST_SIZ(agt->adr1) },
							{ agt->adr2,NULL, FLD_WST, FLD_WST_SIZ(agt->adr2) },
							{ agt->zip,	NULL, FLD_WST, FLD_WST_SIZ(agt->zip) },
							{ agt->tel1,NULL, FLD_WST, FLD_WST_SIZ(agt->tel1) },
							{ agt->tel2,NULL, FLD_WST, FLD_WST_SIZ(agt->tel2) },
							{ agt->fax,	NULL, FLD_WST, FLD_WST_SIZ(agt->fax) },
							{ agt->eml1,NULL, FLD_WST, FLD_WST_SIZ(agt->eml1) },
							{ agt->eml2,NULL, FLD_WST, FLD_WST_SIZ(agt->eml2) },
							{ agt->url,	NULL, FLD_WST, FLD_WST_SIZ(agt->url) },
							{ agt->ctr,	NULL, FLD_WST, FLD_WST_SIZ(agt->ctr) },
							{ agt->rct,	NULL, FLD_INT, FLD_INT_SIZ(agt->rct) },
							{ agt->etc,	NULL, FLD_INT, FLD_INT_SIZ(agt->etc) },
							{ agt->cxc,	NULL, FLD_INT, FLD_INT_SIZ(agt->cxc) },
							{ agt->cmo,	NULL, FLD_INT, FLD_INT_SIZ(agt->cmo) },
							{ agt->mei,	NULL, FLD_INT, FLD_INT_SIZ(agt->mei) },
							{ agt->cdt,	NULL, FLD_DTM, FLD_DTM_SIZ(agt->cdt) },
							{ agt->mdt,	NULL, FLD_DTM, FLD_DTM_SIZ(agt->mdt) },
							{ agt->ddt,	NULL, FLD_DTM, FLD_DTM_SIZ(agt->ddt) } };

	return( mkf_cpy( dst, fld, memcnt(fld), pfx, FDF_AGT_INF, ext, ect ) );
}

int			Fdf_Cdc_Inf(		// Making the creditcard_companies field definition
Dbs_fld			**dst,			// Destination pointer
void			*ptr,			// pointer of target structure
WCHAR			*pfx,			// prefix of each items
Dbs_fld			*ext,			// Extended fields definition
int				ect )			// Extended fields number
{
	CDC_COM		*cdc = (CDC_COM *)ptr;
	Dbs_fld		fld[] = {	{ cdc->id,	NULL, FLD_INT, FLD_INT_SIZ(cdc->id), },
							{ cdc->opc,	NULL, FLD_WST, FLD_WST_SIZ(cdc->opc) },
							{ cdc->ccc,	NULL, FLD_WST, FLD_WST_SIZ(cdc->ccc) },
							{ cdc->nam,	NULL, FLD_WST, FLD_WST_SIZ(cdc->nam) },
//							{ cdc->abr,	NULL, FLD_WST, FLD_WST_SIZ(cdc->abr) },
							{ cdc->adr1,NULL, FLD_WST, FLD_WST_SIZ(cdc->adr1) },
							{ cdc->adr2,NULL, FLD_WST, FLD_WST_SIZ(cdc->adr2) },
							{ cdc->zip,	NULL, FLD_WST, FLD_WST_SIZ(cdc->zip) },
							{ cdc->tel1,NULL, FLD_WST, FLD_WST_SIZ(cdc->tel1) },
							{ cdc->tel2,NULL, FLD_WST, FLD_WST_SIZ(cdc->tel2) },
							{ cdc->fax,	NULL, FLD_WST, FLD_WST_SIZ(cdc->fax) },
							{ cdc->eml1,NULL, FLD_WST, FLD_WST_SIZ(cdc->eml1) },
							{ cdc->eml2,NULL, FLD_WST, FLD_WST_SIZ(cdc->eml2) },
							{ cdc->rte,	NULL, FLD_DBL, FLD_DBL_SIZ(cdc->rte) },
							{ cdc->url,	NULL, FLD_WST, FLD_WST_SIZ(cdc->url) },
							{ cdc->ctr,	NULL, FLD_WST, FLD_WST_SIZ(cdc->ctr) },
							{ cdc->pdy,	NULL, FLD_DAY, FLD_DAY_SIZ(cdc->pdy) },
							{ cdc->cld,	NULL, FLD_DAY, FLD_DAY_SIZ(cdc->cld) },
							{ cdc->mei,	NULL, FLD_INT, FLD_INT_SIZ(cdc->mei) },
							{ cdc->cdt,	NULL, FLD_DTM, FLD_DTM_SIZ(cdc->cdt) },
							{ cdc->mdt,	NULL, FLD_DTM, FLD_DTM_SIZ(cdc->mdt) },
							{ cdc->ddt,	NULL, FLD_DTM, FLD_DTM_SIZ(cdc->ddt) } };

	return( mkf_cpy( dst, fld, memcnt(fld), pfx, FDF_CDC_INF, ext, ect ) );
}

int			Fdf_Cli_Inf(		// Making the client_infos field definition
Dbs_fld			**dst,			// Destination pointer
void			*ptr,			// pointer of target structure
WCHAR			*pfx,			// prefix of each items
Dbs_fld			*ext,			// Extended fields definition
int				ect )			// Extended fields number
{
	CLI_INF		*cli = (CLI_INF *)ptr;
	Dbs_fld		fld[] = {	{ cli->id,	NULL, FLD_INT, FLD_INT_SIZ(cli->id)	},
							{ cli->opc,	NULL, FLD_WST, FLD_WST_SIZ(cli->opc)	},  
							{ cli->ckd,	NULL, FLD_INT, FLD_INT_SIZ(cli->ckd)	},  
							{ cli->fnm,	NULL, FLD_WST, FLD_WST_SIZ(cli->fnm)	},  
							{ cli->lnm,	NULL, FLD_WST, FLD_WST_SIZ(cli->lnm)	},  
							{ cli->gdr,	NULL, FLD_INT, FLD_INT_SIZ(cli->gdr)	},  
							{ cli->bth,	NULL, FLD_DAY, FLD_DAY_SIZ(cli->bth)	},  
							{ cli->adr1,NULL, FLD_WST, FLD_WST_SIZ(cli->adr1)	},  
							{ cli->adr2,NULL, FLD_WST, FLD_WST_SIZ(cli->adr2)	},  
							{ cli->zip,	NULL, FLD_WST, FLD_WST_SIZ(cli->zip)	},  
							{ cli->tel1,NULL, FLD_WST, FLD_WST_SIZ(cli->tel1)	},  
							{ cli->tel2,NULL, FLD_WST, FLD_WST_SIZ(cli->tel2)	},  
							{ cli->fax,	NULL, FLD_WST, FLD_WST_SIZ(cli->fax)	},  
							{ cli->eml1,NULL, FLD_WST, FLD_WST_SIZ(cli->eml1)	},  
							{ cli->eml2,NULL, FLD_WST, FLD_WST_SIZ(cli->eml2)	},  
							{ cli->nto,	NULL, FLD_WST, FLD_WST_SIZ(cli->nto)	},  
							{ cli->lng,	NULL, FLD_WST, FLD_WST_SIZ(cli->lng)	},  
							{ cli->slg,	NULL, FLD_WST, FLD_WST_SIZ(cli->slg)	},  
							{ cli->ccv,	NULL, FLD_INT, FLD_INT_SIZ(cli->ccv)	},  
							{ cli->idn,	NULL, FLD_WST, FLD_WST_SIZ(cli->idn)	},  
							{ cli->isd,	NULL, FLD_DAY, FLD_DAY_SIZ(cli->isd)	},  
							{ cli->exd,	NULL, FLD_DAY, FLD_DAY_SIZ(cli->exd)	},  
							{ cli->vsn,	NULL, FLD_WST, FLD_WST_SIZ(cli->vsn)	},  
							{ cli->vex,	NULL, FLD_DAY, FLD_DAY_SIZ(cli->vex)	},  
							{ cli->crk,	NULL, FLD_INT, FLD_INT_SIZ(cli->crk)	},  
							{ cli->mrd,	NULL, FLD_INT, FLD_INT_SIZ(cli->mrd)	},  
							{ cli->fve1,NULL, FLD_INT, FLD_INT_SIZ(cli->fve1)	},  
							{ cli->fve2,NULL, FLD_INT, FLD_INT_SIZ(cli->fve2)	},  
							{ cli->usc,	NULL, FLD_INT, FLD_INT_SIZ(cli->usc)	},  
							{ cli->nsc,	NULL, FLD_INT, FLD_INT_SIZ(cli->nsc)	},  
							{ cli->pip,	NULL, FLD_WST, FLD_WST_SIZ(cli->pip)	},  
							{ cli->ism,	NULL, FLD_INT, FLD_INT_SIZ(cli->ism)	},  
							{ cli->mbi,	NULL, FLD_WST, FLD_WST_SIZ(cli->mbi)	},  
							{ cli->txc,	NULL, FLD_WST, FLD_WST_SIZ(cli->txc)	},  
							{ cli->ccc,	NULL, FLD_INT, FLD_INT_SIZ(cli->ccc)	},  
							{ cli->ccn,	NULL, FLD_WST, FLD_WST_SIZ(cli->ccn)	},  
							{ cli->cct,	NULL, FLD_WST, FLD_WST_SIZ(cli->cct)	},  
							{ cli->mei,	NULL, FLD_INT, FLD_INT_SIZ(cli->mei)	},  
							{ cli->cdt,	NULL, FLD_DTM, FLD_DTM_SIZ(cli->cdt)	},  
							{ cli->mdt,	NULL, FLD_DTM, FLD_DTM_SIZ(cli->mdt)	}, 
							{ cli->ddt,	NULL, FLD_DTM, FLD_DTM_SIZ(cli->ddt)	} };

	return( mkf_cpy( dst, fld, memcnt(fld), pfx, FDF_CLI_INF, ext, ect ) );
}

int			Fdf_Crc_Cvs(		// Making the currency_conversions field definition
Dbs_fld			**dst,			// Destination pointer
void			*ptr,			// pointer of target structure
WCHAR			*pfx,			// prefix of each items
Dbs_fld			*ext,			// Extended fields definition
int				ect )			// Extended fields number
{
	CRC_CVS		*cvs = (CRC_CVS *)ptr;
	Dbs_fld		fld[] = {	{ cvs->id,	NULL, FLD_INT, FLD_INT_SIZ(cvs->id)	},
							{ cvs->opc,	NULL, FLD_WST, FLD_WST_SIZ(cvs->opc)},
							{ cvs->brc,	NULL, FLD_WST, FLD_WST_SIZ(cvs->brc)},
							{ cvs->fcc,	NULL, FLD_WST, FLD_WST_SIZ(cvs->fcc)},
							{ cvs->crc,	NULL, FLD_WST, FLD_WST_SIZ(cvs->crc)},
							{ cvs->nam,	NULL, FLD_WST, FLD_WST_SIZ(cvs->nam)},
							{ cvs->isb,	NULL, FLD_INT, FLD_INT_SIZ(cvs->isb)},
							{ cvs->iss,	NULL, FLD_INT, FLD_INT_SIZ(cvs->iss)},
							{ cvs->adp,	NULL, FLD_INT, FLD_INT_SIZ(cvs->adp)},
							{ cvs->rte,	NULL, FLD_DBL, FLD_DBL_SIZ(cvs->rte)},
							{ cvs->mei,	NULL, FLD_INT, FLD_INT_SIZ(cvs->mei)},
							{ cvs->cdt,	NULL, FLD_DTM, FLD_DTM_SIZ(cvs->cdt)},
							{ cvs->mdt,	NULL, FLD_DTM, FLD_DTM_SIZ(cvs->mdt)},
							{ cvs->ddt,	NULL, FLD_DTM, FLD_DTM_SIZ(cvs->ddt)} };

	return( mkf_cpy( dst, fld, memcnt(fld), pfx, FDF_CRC_CVS, ext, ect ) );
}

int			Fdf_Rsv_Inf(		// Making the reservation_infos field definition
Dbs_fld			**dst,			// Destination pointer
void			*ptr,			// pointer of target structure
WCHAR			*pfx,			// prefix of each items
Dbs_fld			*ext,			// Extended fields definition
int				ect )			// Extended fields number
{
	RSV_INF		*rsv = (RSV_INF *)ptr;
	Dbs_fld		fld[] = {	{ rsv->id,	NULL, FLD_INT, FLD_INT_SIZ(rsv->id)},
							{ rsv->opc,	NULL, FLD_WST, FLD_WST_SIZ(rsv->opc)},
							{ rsv->brc,	NULL, FLD_WST, FLD_WST_SIZ(rsv->brc)},
							{ rsv->fcc,	NULL, FLD_WST, FLD_WST_SIZ(rsv->fcc)},
							{ rsv->rvn,	NULL, FLD_INT, FLD_INT_SIZ(rsv->rvn)},
							{ rsv->cii,	NULL, FLD_INT, FLD_INT_SIZ(rsv->cii)},
							{ rsv->rpm,	NULL, FLD_WST, FLD_WST_SIZ(rsv->rpm)},
							{ rsv->rmg,	NULL, FLD_INT, FLD_INT_SIZ(rsv->rmg)},
							{ rsv->aii,	NULL, FLD_INT, FLD_INT_SIZ(rsv->aii)},
							{ rsv->olr,	NULL, FLD_INT, FLD_INT_SIZ(rsv->olr)},
							{ rsv->ern,	NULL, FLD_WST, FLD_WST_SIZ(rsv->ern)},
							{ rsv->arv,	NULL, FLD_DAY, FLD_DAY_SIZ(rsv->arv)},
							{ rsv->dpt,	NULL, FLD_DAY, FLD_DAY_SIZ(rsv->dpt)},
							{ rsv->cxl,	NULL, FLD_INT, FLD_INT_SIZ(rsv->cxl)},
							{ rsv->nsw,	NULL, FLD_INT, FLD_INT_SIZ(rsv->nsw)},
							{ rsv->cis,	NULL, FLD_INT, FLD_INT_SIZ(rsv->cis)},
							{ rsv->bfr,	NULL, FLD_INT, FLD_INT_SIZ(rsv->bfr)},
							{ rsv->nte,	NULL, FLD_WST, FLD_WST_SIZ(rsv->nte)},
							{ rsv->rcd,	NULL, FLD_DAY, FLD_DAY_SIZ(rsv->rcd)},
							{ rsv->msi,	NULL, FLD_INT, FLD_INT_SIZ(rsv->msi)},
							{ rsv->pym,	NULL, FLD_INT, FLD_INT_SIZ(rsv->pym)},
							{ rsv->mei,	NULL, FLD_INT, FLD_INT_SIZ(rsv->mei)},
							{ rsv->cdt,	NULL, FLD_DTM, FLD_DTM_SIZ(rsv->cdt)},
							{ rsv->mdt,	NULL, FLD_DTM, FLD_DTM_SIZ(rsv->mdt)},
							{ rsv->ddt,	NULL, FLD_DTM, FLD_DTM_SIZ(rsv->ddt)} };

	return( mkf_cpy( dst, fld, memcnt(fld), pfx, FDF_RSV_INF, ext, ect ) );
}

int			Fdf_Rsv_Dtl(		// Making the reservation_infos field definition
Dbs_fld			**dst,			// Destination pointer
void			*ptr,			// pointer of target structure
WCHAR			*pfx,			// prefix of each items
Dbs_fld			*ext,			// Extended fields definition
int				ect )			// Extended fields number
{
	RSV_DTL		*dtl = (RSV_DTL *)ptr;
	Dbs_fld		fld[] = {	{ dtl->id,	NULL, FLD_INT, FLD_INT_SIZ(dtl->id)},		
							{ dtl->opc,	NULL, FLD_WST, FLD_WST_SIZ(dtl->opc)},
							{ dtl->rii,	NULL, FLD_INT, FLD_INT_SIZ(dtl->rii)},		
							{ dtl->cii,	NULL, FLD_INT, FLD_INT_SIZ(dtl->cii)},	
							{ dtl->arv,	NULL, FLD_DAY, FLD_DAY_SIZ(dtl->arv)},	
							{ dtl->atm,	NULL, FLD_TIM, FLD_TIM_SIZ(dtl->atm)},	
							{ dtl->dpt,	NULL, FLD_DAY, FLD_DAY_SIZ(dtl->dpt)},	
							{ dtl->dtm,	NULL, FLD_TIM, FLD_TIM_SIZ(dtl->dtm)},	
							{ dtl->cid,	NULL, FLD_DTM, FLD_DTM_SIZ(dtl->cid)},	
							{ dtl->cod,	NULL, FLD_DTM, FLD_DTM_SIZ(dtl->cod)},	
							{ dtl->eci,	NULL, FLD_INT, FLD_INT_SIZ(dtl->eci)},	
							{ dtl->lco,	NULL, FLD_INT, FLD_INT_SIZ(dtl->lco)},	
							{ dtl->psn,	NULL, FLD_INT, FLD_INT_SIZ(dtl->psn)},	
							{ dtl->chn,	NULL, FLD_INT, FLD_INT_SIZ(dtl->chn)},	
							{ dtl->eti,	NULL, FLD_INT, FLD_INT_SIZ(dtl->eti)},	
							{ dtl->inh,	NULL, FLD_INT, FLD_INT_SIZ(dtl->inh)},	
							{ dtl->cxl,	NULL, FLD_INT, FLD_INT_SIZ(dtl->cxl)},	
							{ dtl->nte,	NULL, FLD_WST, FLD_WST_SIZ(dtl->nte)},	
							{ dtl->mei,	NULL, FLD_INT, FLD_INT_SIZ(dtl->mei)},
							{ dtl->cdt,	NULL, FLD_DTM, FLD_DTM_SIZ(dtl->cdt)},	
							{ dtl->mdt,	NULL, FLD_DTM, FLD_DTM_SIZ(dtl->mdt)},	
							{ dtl->ddt,	NULL, FLD_DTM, FLD_DTM_SIZ(dtl->ddt)} };	

	return( mkf_cpy( dst, fld, memcnt(fld), pfx, FDF_RSV_DTL, ext, ect ) );
}

int			Fdf_Rsv_Eqp(		// Making the reservation_equipments field definition
Dbs_fld			**dst,			// Destination pointer
void			*ptr,			// pointer of target structure
WCHAR			*pfx,			// prefix of each items
Dbs_fld			*ext,			// Extended fields definition
int				ect )			// Extended fields number
{
	RSV_EQP		*req = (RSV_EQP *)ptr;
	Dbs_fld		fld[] = {	{ req->id,	NULL, FLD_INT, FLD_INT_SIZ(req->id)},		
							{ req->opc,	NULL, FLD_WST, FLD_WST_SIZ(req->opc)},
							{ req->rii,	NULL, FLD_INT, FLD_INT_SIZ(req->rii)},		
							{ req->rdi,	NULL, FLD_INT, FLD_INT_SIZ(req->rdi)},	
							{ req->eti,	NULL, FLD_INT, FLD_INT_SIZ(req->eti)},	
							{ req->eii,	NULL, FLD_INT, FLD_INT_SIZ(req->eii)},	
							{ req->usd,	NULL, FLD_DAY, FLD_DAY_SIZ(req->usd)},	
							{ req->ued,	NULL, FLD_DAY, FLD_DAY_SIZ(req->ued)},	
							{ req->inh,	NULL, FLD_INT, FLD_INT_SIZ(req->inh)},	
							{ req->lud,	NULL, FLD_DAY, FLD_DAY_SIZ(req->lud)},	
							{ req->dsi,	NULL, FLD_INT, FLD_INT_SIZ(req->dsi)},	
							{ req->aam,	NULL, FLD_INT, FLD_INT_SIZ(req->aam)},	
							{ req->mei,	NULL, FLD_INT, FLD_INT_SIZ(req->mei)},	
							{ req->cdt,	NULL, FLD_DTM, FLD_DTM_SIZ(req->cdt)},	
							{ req->mdt,	NULL, FLD_DTM, FLD_DTM_SIZ(req->mdt)},	
							{ req->ddt,	NULL, FLD_DTM, FLD_DTM_SIZ(req->ddt)} };	

	return( mkf_cpy( dst, fld, memcnt(fld), pfx, FDF_RSV_EQP, ext, ect ) );
}

int			Fdf_Rsv_Crg(		// Making the reservation_charges field definition
Dbs_fld			**dst,			// Destination pointer
void			*ptr,			// pointer of target structure
WCHAR			*pfx,			// prefix of each items
Dbs_fld			*ext,			// Extended fields definition
int				ect )			// Extended fields number
{
	RSV_CRG		*rcg = (RSV_CRG *)ptr;
	Dbs_fld		fld[] = {	{ rcg->id,	NULL, FLD_INT, FLD_INT_SIZ(rcg->id) },		
							{ rcg->opc,	NULL, FLD_WST, FLD_WST_SIZ(rcg->opc)},
							{ rcg->rii,	NULL, FLD_INT, FLD_INT_SIZ(rcg->rii)},		
							{ rcg->rdi,	NULL, FLD_INT, FLD_INT_SIZ(rcg->rdi)},	
							{ rcg->rei,	NULL, FLD_INT, FLD_INT_SIZ(rcg->rei)},	
							{ rcg->udt,	NULL, FLD_DAY, FLD_DAY_SIZ(rcg->udt)},	
							{ rcg->cgk,	NULL, FLD_INT, FLD_INT_SIZ(rcg->cgk)},	
							{ rcg->dsi,	NULL, FLD_INT, FLD_INT_SIZ(rcg->dsi)},	
							{ rcg->edi,	NULL, FLD_INT, FLD_INT_SIZ(rcg->edi)},	
							{ rcg->amt,	NULL, FLD_DBL, FLD_DBL_SIZ(rcg->amt)},	
							{ rcg->pup,	NULL, FLD_INT, FLD_INT_SIZ(rcg->pup)},	
							{ rcg->mei,	NULL, FLD_INT, FLD_INT_SIZ(rcg->mei)},	
							{ rcg->cdt,	NULL, FLD_DTM, FLD_DTM_SIZ(rcg->cdt)},	
							{ rcg->mdt,	NULL, FLD_DTM, FLD_DTM_SIZ(rcg->mdt)},	
							{ rcg->ddt,	NULL, FLD_DTM, FLD_DTM_SIZ(rcg->ddt)} };

	return( mkf_cpy( dst, fld, memcnt(fld), pfx, FDF_RSV_CRG, ext, ect ) );
}

int			Fdf_Dsc_Rpt(		// Making the descriptions field definition
Dbs_fld			**dst,			// Destination pointer
void			*ptr,			// pointer of target structure
WCHAR			*pfx,			// prefix of each items
Dbs_fld			*ext,			// Extended fields definition
int				ect )			// Extended fields number
{
	DSC_RPT		*dsc = (DSC_RPT *)ptr;
	Dbs_fld		fld[] = {	{ dsc->id,	NULL, FLD_INT, FLD_INT_SIZ(dsc->id)},		
							{ dsc->opc,	NULL, FLD_WST, FLD_WST_SIZ(dsc->opc)},
							{ dsc->brc,	NULL, FLD_WST, FLD_WST_SIZ(dsc->brc)},
							{ dsc->fcc,	NULL, FLD_WST, FLD_WST_SIZ(dsc->fcc)},
							{ dsc->dsc,	NULL, FLD_WST, FLD_WST_SIZ(dsc->dsc)},
							{ dsc->dkd,	NULL, FLD_INT, FLD_INT_SIZ(dsc->dkd)},
							{ dsc->dci,	NULL, FLD_INT, FLD_INT_SIZ(dsc->dci)},
							{ dsc->nam,	NULL, FLD_WST, FLD_WST_SIZ(dsc->nam)},
							{ dsc->abr,	NULL, FLD_WST, FLD_WST_SIZ(dsc->abr)},
							{ dsc->vss,	NULL, FLD_DAY, FLD_DAY_SIZ(dsc->vss)},
							{ dsc->vse,	NULL, FLD_DAY, FLD_DAY_SIZ(dsc->vse)},
							{ dsc->prk,	NULL, FLD_INT, FLD_INT_SIZ(dsc->prk)},
							{ dsc->prc,	NULL, FLD_DBL, FLD_DBL_SIZ(dsc->prc)},
							{ dsc->apl,	NULL, FLD_INT, FLD_INT_SIZ(dsc->apl)},
							{ dsc->atr,	NULL, FLD_INT, FLD_INT_SIZ(dsc->atr)},
							{ dsc->sen,	NULL, FLD_INT, FLD_INT_SIZ(dsc->sen)},
							{ dsc->rac,	NULL, FLD_WST, FLD_WST_SIZ(dsc->rac)},
							{ dsc->tfi,	NULL, FLD_INT, FLD_INT_SIZ(dsc->tfi)},
							{ dsc->mei,	NULL, FLD_INT, FLD_INT_SIZ(dsc->mei)},
							{ dsc->cdt,	NULL, FLD_DTM, FLD_DTM_SIZ(dsc->cdt)},
							{ dsc->mdt,	NULL, FLD_DTM, FLD_DTM_SIZ(dsc->mdt)},
							{ dsc->ddt,	NULL, FLD_DTM, FLD_DTM_SIZ(dsc->ddt)} };

	return( mkf_cpy( dst, fld, memcnt(fld), pfx, FDF_DSC_RPT, ext, ect ) );
}

int			Fdf_Dsc_Cat(		// Making the description_categories field definition
Dbs_fld			**dst,			// Destination pointer
void			*ptr,			// pointer of target structure
WCHAR			*pfx,			// prefix of each items
Dbs_fld			*ext,			// Extended fields definition
int				ect )			// Extended fields number
{
	DSC_CAT		*cat = (DSC_CAT *)ptr;
	Dbs_fld		fld[] = {	{ cat->id,	NULL, FLD_INT, FLD_INT_SIZ(cat->id)},		
							{ cat->opc,	NULL, FLD_WST, FLD_WST_SIZ(cat->opc)},
							{ cat->brc,	NULL, FLD_WST, FLD_WST_SIZ(cat->brc)},
							{ cat->fcc,	NULL, FLD_WST, FLD_WST_SIZ(cat->fcc)},
							{ cat->nam,	NULL, FLD_WST, FLD_WST_SIZ(cat->nam)},
							{ cat->abr,	NULL, FLD_WST, FLD_WST_SIZ(cat->abr)},
							{ cat->odr,	NULL, FLD_INT, FLD_INT_SIZ(cat->odr)},
							{ cat->mei,	NULL, FLD_INT, FLD_INT_SIZ(cat->mei)},
							{ cat->cdt,	NULL, FLD_DTM, FLD_DTM_SIZ(cat->cdt)},
							{ cat->mdt,	NULL, FLD_DTM, FLD_DTM_SIZ(cat->mdt)},
							{ cat->ddt,	NULL, FLD_DTM, FLD_DTM_SIZ(cat->ddt)} };

	return( mkf_cpy( dst, fld, memcnt(fld), pfx, FDF_DSC_CAT, ext, ect ) );
}

int			Fdf_Eqp_Crg(		// Making the equipment_charge_infos field definition
Dbs_fld			**dst,			// Destination pointer
void			*ptr,			// pointer of target structure
WCHAR			*pfx,			// prefix of each items
Dbs_fld			*ext,			// Extended fields definition
int				ect )			// Extended fields number
{
	EQP_CRG		*ecg = (EQP_CRG *)ptr;
	Dbs_fld		fld[] = {	{ ecg->id,	NULL, FLD_INT, FLD_INT_SIZ(ecg->id)},		
							{ ecg->opc,	NULL, FLD_WST, FLD_WST_SIZ(ecg->opc)},
							{ ecg->brc,	NULL, FLD_WST, FLD_WST_SIZ(ecg->brc)},
							{ ecg->fcc,	NULL, FLD_WST, FLD_WST_SIZ(ecg->fcc)},
							{ ecg->eti,	NULL, FLD_INT, FLD_INT_SIZ(ecg->eti)},
							{ ecg->eii,	NULL, FLD_INT, FLD_INT_SIZ(ecg->eii)},
							{ ecg->aii,	NULL, FLD_INT, FLD_INT_SIZ(ecg->aii)},
							{ ecg->dsi,	NULL, FLD_INT, FLD_INT_SIZ(ecg->dsi)},
							{ ecg->cgk,	NULL, FLD_INT, FLD_INT_SIZ(ecg->cgk)},
							{ ecg->cgp,	NULL, FLD_INT, FLD_INT_SIZ(ecg->cgp)},
							{ ecg->bfr,	NULL, FLD_INT, FLD_INT_SIZ(ecg->bfr)},
							{ ecg->psn,	NULL, FLD_INT, FLD_INT_SIZ(ecg->psn)},
							{ ecg->cmr,	NULL, FLD_INT, FLD_INT_SIZ(ecg->cmr)},
							{ ecg->cmn,	NULL, FLD_INT, FLD_INT_SIZ(ecg->cmn)},
							{ ecg->cmx,	NULL, FLD_INT, FLD_INT_SIZ(ecg->cmx)},
							{ ecg->mei,	NULL, FLD_INT, FLD_INT_SIZ(ecg->mei)},
							{ ecg->cdt,	NULL, FLD_DTM, FLD_DTM_SIZ(ecg->cdt)},
							{ ecg->mdt,	NULL, FLD_DTM, FLD_DTM_SIZ(ecg->mdt)},
							{ ecg->ddt,	NULL, FLD_DTM, FLD_DTM_SIZ(ecg->ddt)} };

	return( mkf_cpy( dst, fld, memcnt(fld), pfx, FDF_EQP_CRG, ext, ect ) );
}

int			Fdf_Eqp_Cgd(		// Making the equipment_charge_details field definition
Dbs_fld			**dst,			// Destination pointer
void			*ptr,			// pointer of target structure
WCHAR			*pfx,			// prefix of each items
Dbs_fld			*ext,			// Extended fields definition
int				ect )			// Extended fields number
{
	EQP_CGD		*ecd = (EQP_CGD *)ptr;
	Dbs_fld		fld[] = {	{ ecd->id,	NULL, FLD_INT, FLD_INT_SIZ(ecd->id)},		
							{ ecd->opc,	NULL, FLD_WST, FLD_WST_SIZ(ecd->opc)},
							{ ecd->brc,	NULL, FLD_WST, FLD_WST_SIZ(ecd->brc)},
							{ ecd->fcc,	NULL, FLD_WST, FLD_WST_SIZ(ecd->fcc)},
							{ ecd->eci,	NULL, FLD_INT, FLD_INT_SIZ(ecd->eci)},
							{ ecd->prc,	NULL, FLD_DBL, FLD_DBL_SIZ(ecd->prc)},
							{ ecd->adk,	NULL, FLD_INT, FLD_INT_SIZ(ecd->adk)},
							{ ecd->mei,	NULL, FLD_INT, FLD_INT_SIZ(ecd->mei)},
							{ ecd->cdt,	NULL, FLD_DTM, FLD_DTM_SIZ(ecd->cdt)},
							{ ecd->mdt,	NULL, FLD_DTM, FLD_DTM_SIZ(ecd->mdt)},
							{ ecd->ddt,	NULL, FLD_DTM, FLD_DTM_SIZ(ecd->ddt)} };

	return( mkf_cpy( dst, fld, memcnt(fld), pfx, FDF_EQP_CGD, ext, ect ) );
}

int			Fdf_Sal_Inf(		// Making the sales_infos field definition
Dbs_fld			**dst,			// Destination pointer
void			*ptr,			// pointer of target structure
WCHAR			*pfx,			// prefix of each items
Dbs_fld			*ext,			// Extended fields definition
int				ect )			// Extended fields number
{
	SAL_INF		*sal = (SAL_INF *)ptr;
	Dbs_fld		fld[] = {	{ sal->id,	NULL, FLD_INT, FLD_INT_SIZ(sal->id)	},
							{ sal->opc,	NULL, FLD_WST, FLD_WST_SIZ(sal->opc)},
							{ sal->rii,	NULL, FLD_INT, FLD_INT_SIZ(sal->rii)},
							{ sal->rdi,	NULL, FLD_INT, FLD_INT_SIZ(sal->rdi)},
							{ sal->eii,	NULL, FLD_INT, FLD_INT_SIZ(sal->eii)},
							{ sal->brc,	NULL, FLD_WST, FLD_WST_SIZ(sal->brc)},
							{ sal->fcc,	NULL, FLD_WST, FLD_WST_SIZ(sal->fcc)},
							{ sal->ivn,	NULL, FLD_INT, FLD_INT_SIZ(sal->ivn)},
							{ sal->flg,	NULL, FLD_INT, FLD_INT_SIZ(sal->flg)},
							{ sal->pid,	NULL, FLD_INT, FLD_INT_SIZ(sal->pid)},
							{ sal->mei,	NULL, FLD_INT, FLD_INT_SIZ(sal->mei)},
							{ sal->cdt,	NULL, FLD_DTM, FLD_DTM_SIZ(sal->cdt)},
							{ sal->mdt,	NULL, FLD_DTM, FLD_DTM_SIZ(sal->mdt)},
							{ sal->ddt,	NULL, FLD_DTM, FLD_DTM_SIZ(sal->ddt)} };

	return( mkf_cpy( dst, fld, memcnt(fld), pfx, FDF_SAL_INF, ext, ect ) );
}

int			Fdf_Sal_Dtl(		// Making the sales_details field definition
Dbs_fld			**dst,			// Destination pointer
void			*ptr,			// pointer of target structure
WCHAR			*pfx,			// prefix of each items
Dbs_fld			*ext,			// Extended fields definition
int				ect )			// Extended fields number
{
	SAL_DTL		*dtl = (SAL_DTL *)ptr;
	Dbs_fld		fld[] = {	{ dtl->id,	NULL, FLD_INT, FLD_INT_SIZ(dtl->id)	},
							{ dtl->opc,	NULL, FLD_WST, FLD_WST_SIZ(dtl->opc)},
							{ dtl->sii,	NULL, FLD_INT, FLD_INT_SIZ(dtl->sii)},
							{ dtl->sld,	NULL, FLD_DAY, FLD_DAY_SIZ(dtl->sld)},
							{ dtl->sdt,	NULL, FLD_DTM, FLD_DTM_SIZ(dtl->sdt)},
							{ dtl->dsi,	NULL, FLD_INT, FLD_INT_SIZ(dtl->dsi)},
							{ dtl->qty,	NULL, FLD_DBL, FLD_DBL_SIZ(dtl->qty)},
							{ dtl->nmp,	NULL, FLD_DBL, FLD_DBL_SIZ(dtl->nmp)},
							{ dtl->slp,	NULL, FLD_DBL, FLD_DBL_SIZ(dtl->slp)},
							{ dtl->asi,	NULL, FLD_INT, FLD_INT_SIZ(dtl->asi)},
							{ dtl->eqi,	NULL, FLD_INT, FLD_INT_SIZ(dtl->eqi)},
							{ dtl->mei,	NULL, FLD_INT, FLD_INT_SIZ(dtl->mei)},
							{ dtl->cdt,	NULL, FLD_DTM, FLD_DTM_SIZ(dtl->cdt)},
							{ dtl->mdt,	NULL, FLD_DTM, FLD_DTM_SIZ(dtl->mdt)},
							{ dtl->ddt,	NULL, FLD_DTM, FLD_DTM_SIZ(dtl->ddt)} };

	return( mkf_cpy( dst, fld, memcnt(fld), pfx, FDF_SAL_DTL, ext, ect ) );
}

int			Fdf_Pmt_Dtl(		// Making the payment_details field definition
Dbs_fld			**dst,			// Destination pointer
void			*ptr,			// pointer of target structure
WCHAR			*pfx,			// prefix of each items
Dbs_fld			*ext,			// Extended fields definition
int				ect )			// Extended fields number
{
	PMT_DTL		*dtl = (PMT_DTL *)ptr;
	Dbs_fld		fld[] = {	{ dtl->id,	NULL, FLD_INT, FLD_INT_SIZ(dtl->id)	},
							{ dtl->opc,	NULL, FLD_WST, FLD_WST_SIZ(dtl->opc)},
							{ dtl->sii,	NULL, FLD_INT, FLD_INT_SIZ(dtl->sii)},
							{ dtl->sdi,	NULL, FLD_INT, FLD_INT_SIZ(dtl->sdi)},
							{ dtl->pmd,	NULL, FLD_DAY, FLD_DAY_SIZ(dtl->pmd)},
							{ dtl->pdt,	NULL, FLD_DTM, FLD_DTM_SIZ(dtl->pdt)},
							{ dtl->cci,	NULL, FLD_INT, FLD_INT_SIZ(dtl->cci)},
							{ dtl->rte,	NULL, FLD_DBL, FLD_DBL_SIZ(dtl->rte)},
							{ dtl->amt,	NULL, FLD_DBL, FLD_DBL_SIZ(dtl->amt)},
							{ dtl->mtd,	NULL, FLD_INT, FLD_INT_SIZ(dtl->mtd)},
							{ dtl->dpk,	NULL, FLD_INT, FLD_INT_SIZ(dtl->dpk)},
							{ dtl->mei,	NULL, FLD_INT, FLD_INT_SIZ(dtl->mei)},
							{ dtl->cdt,	NULL, FLD_DTM, FLD_DTM_SIZ(dtl->cdt)},
							{ dtl->mdt,	NULL, FLD_DTM, FLD_DTM_SIZ(dtl->mdt)},
							{ dtl->ddt,	NULL, FLD_DTM, FLD_DTM_SIZ(dtl->ddt)} };

	return( mkf_cpy( dst, fld, memcnt(fld), pfx, FDF_PMT_DTL, ext, ect ) );
}
