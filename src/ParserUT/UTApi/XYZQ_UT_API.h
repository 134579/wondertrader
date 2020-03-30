// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the XYZQ_UT_API_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// XYZQ_UT_API_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#pragma once

#define XYZQ_UT_API

#ifdef __linux
#define WINAPI
typedef int BOOL;
#define __stdcall
#endif

#include "XYZQ_UT_datatype.h"
#include "XYZQ_UT_constant.h"
#include <string.h>

// ������C�ӿڣ����ٶ��нϸ�Ҫ����û�����ѡ��
#ifdef __cplusplus
extern "C" {
#endif
	//ע���¼��ַ
	int XYZQ_UT_API WINAPI ut_register_address(char* address);

	//ע��ص�����
	int XYZQ_UT_API WINAPI ut_register_spi(xyzq_ut_c_spi spi);

	//��ʼ��
	void XYZQ_UT_API WINAPI ut_init();

	//��¼��֤
	int XYZQ_UT_API WINAPI ut_authorize(const char* account, const char* username, const char* password);

    //ȡ����״̬
    BOOL XYZQ_UT_API WINAPI ut_isconnected();

	//�ǳ�
	int XYZQ_UT_API WINAPI ut_logout();
    
    int XYZQ_UT_API WINAPI ut_release();

    int XYZQ_UT_API WINAPI ut_free_object(int object_type, void* object);

    //�ص�¼
    int XYZQ_UT_API WINAPI ut_relogin();

    //�޸�����
	int XYZQ_UT_API WINAPI ut_change_password(const char* account, const char* username, const char* oldpassword, const char* newpassword);

    //����
    int XYZQ_UT_API WINAPI ut_subscribe(sub_item_c* sitem);  // ��Ʒ�ֶ���
    int XYZQ_UT_API WINAPI ut_subscribe_multi(int code_count, ut_code_string* codes, sub_fields_c* fields);  // ��Ʒ����ͬ�ֶζ���
    int XYZQ_UT_API WINAPI ut_subscribe_multi_ex(int code_count, sub_item_c* plist[]);  // ��Ʒ�ֲ�ͬ�ֶζ���
    
    //�����������
    int XYZQ_UT_API WINAPI ut_subscribe_transactions(int code_count, ut_code_string* codes);

    //����ί������
    int XYZQ_UT_API WINAPI ut_subscribe_orders(int code_count, ut_code_string* codes);

    //���Ĺҵ�����
    int XYZQ_UT_API WINAPI ut_subscribe_order_queues(int code_count, ut_code_string* codes);
	
    //��ÿ���
	int XYZQ_UT_API WINAPI ut_get_snapshot(int code_count, sub_item_c* plist[]);
	int XYZQ_UT_API WINAPI ut_get_snapshot_sync(int code_count, sub_item_c* plist[], ut_data_list_c** result, int timeout);

    //���֤ȯ������Ϣ
	int XYZQ_UT_API WINAPI ut_get_baseinfo(int code_count, ut_code_string* codes);
	
    //�˶�
	int XYZQ_UT_API WINAPI ut_unsubscribe(int sub_type, int code_count, ut_code_string* codes);
    int XYZQ_UT_API WINAPI ut_unsubscribe_all(int sub_type);

    //����/��ȡ�û�����
    int XYZQ_UT_API WINAPI ut_set_config(int config_item, int value);
    int XYZQ_UT_API WINAPI ut_get_config(int config_item, int* value);

#ifdef __cplusplus
};
#endif

// ������C++�ӿ�
#ifdef __cplusplus

class xyzq_ut_spi 
{
public:

	//������Ϣ��Ӧ
    virtual void rsp_error(error_msg *arg) {};
	//״̬��Ӧ
    virtual void rsp_status_changed(int status) {};
	//����Ʒ����������
    virtual void rsp_market_data(ut_data *arg) {};
	//���Ʒ����������
    virtual void rsp_market_data_ex(ut_data_list *arg) {};
	//����Ʒ����ʳɽ�����
    virtual void rsp_transactions(ut_data *arg) {};
    //����Ʒ�����ί������
    virtual void rsp_orders(ut_data *arg) {};
	//����Ʒ��ί�ж�������
    virtual void rsp_order_list(ut_order_queue_list *arg) {};
	//��ѯ������Ϣ��Ӧ
    virtual void rsp_base_info(rsp_base_info_s *arg) {};
};

#ifdef __linux
#define strcpy_s(dest, max_len, src) strcpy(dest, src)
#endif

class xyzq_ut_api
{
public:
	//����api
	static xyzq_ut_api *create_ut_api()
    {
        static xyzq_ut_api instance;
        return &instance;
    };

	//ע���¼��ַ
	int register_address(char* address)
    {
        return ut_register_address(address);
    }

	//ע��ص���
	int register_spi(xyzq_ut_spi *spi)
    {
        custom_spi(spi);
        return ut_register_spi(virtual_callback);
    }

	//��ʼ��
	void init()
    {
        return ut_init();
    }

	//��¼��֤
	int authorize(authorize_struct *as)
    {
        return ut_authorize(as->account_no.c_str(), as->username.c_str(), as->password.c_str());
    }

    //ȡ����״̬
    bool isconnected()
    {
        return ut_isconnected();
    }

	//�ǳ�
	int logout()
    {
        return ut_logout();
    }
    
    //�ص�¼
    int relogin()
    {
        return ut_relogin();
    }

    //�޸�����
	int change_password(change_pwd *cp)
    {
        return ut_change_password(  cp->account_no.c_str(), 
                                    cp->username.c_str(), 
                                    cp->oldpassword.c_str(),
                                    cp->newpassword.c_str() );
    }

	//����

    // ��Ʒ�ֶ���
    int subscribe(sub_item_c* sitem)
    {
        return ut_subscribe(sitem);
    }

    // ��Ʒ�ֶ���
    int subscribe(sub_item* sitem)
    {
        if (!sitem)
            return XYUT_ERR_BAD_API_PARAMETERS;
        int n = sitem->fields.size();
        if (n <= 0)
            return XYUT_ERR_BAD_API_PARAMETERS;
        sub_item_c* citem = (sub_item_c*) malloc(sizeof(sub_item_c) + (n-1)*sizeof(int));
        citem->fields.count = n;
        memcpy(citem->fields.fields, &sitem->fields[0], sizeof(int) * n);
        strcpy_s(citem->code, max_code_length, sitem->code.c_str());

        int ret = ut_subscribe(citem);
        free(citem);
        return ret;
    }

    // ��Ʒ����ͬ�ֶζ���
    int subscribe(int code_count, ut_code_string* codes, sub_fields_c* fields)
    {
        return ut_subscribe_multi(code_count, codes, fields);
    }

    // ��Ʒ����ͬ�ֶζ���
    int subscribe(code_list* codes, sub_fields* fields)
    {
        int code_count = codes->size();
        int fn = fields->size();
        if (code_count == 0 || fn == 0)
            return XYUT_ERR_BAD_API_PARAMETERS;

        ut_code_string* sub_codes = new ut_code_string[code_count];//(ut_code_string*) malloc(sizeof(ut_code_string) * code_count);
        for (int i = 0; i < code_count; i++)
        {
            strcpy_s(sub_codes[i], max_code_length, codes->at(i).c_str());
        }

        sub_fields_c* sub_fields = (sub_fields_c*) malloc(sizeof(sub_fields_c) + (fn-1) * sizeof(int));
        sub_fields->count = fn;
        memcpy(sub_fields->fields, &fields->at(0), sizeof(int) * fn);
        
        int ret = ut_subscribe_multi(code_count, sub_codes, sub_fields);
        free(sub_fields);
        delete [] sub_codes;
        return ret;
    }

    // ��Ʒ�ֲ�ͬ�ֶζ���
    int subscribe(sub_list* slist)  
    {
        int code_count = slist->size();
        if (code_count == 0)
            return XYUT_OK;

        sub_item_c** items = new sub_item_c*[code_count];

        for (int i = 0; i < code_count; i++)
        {
            int fn = slist->at(i).fields.size();
            items[i] = (sub_item_c*)malloc(sizeof(sub_item_c) + (fn-1) * sizeof(sub_fields_c));
            strcpy_s(items[i]->code, max_code_length, slist->at(i).code.c_str());
            items[i]->fields.count = fn;
            memcpy(items[i]->fields.fields, &(slist->at(i).fields[0]), fn * sizeof(int));
        }

        int ret = ut_subscribe_multi_ex(code_count, items);
        
        for(int i = 0; i < code_count; i++)
        {
            free(items[i]);
        }
        delete []items;

        return ret;
    }
    
    //������ʳɽ�����
    int subscribe_transactions(code_list* codes)
    {
        int code_count = codes->size();
        if (code_count == 0)
            return XYUT_OK;

        ut_code_string* sub_codes = new ut_code_string[code_count];//(ut_code_string*) malloc(sizeof(ut_code_string) * code_count);
        for (int i = 0; i < code_count; i++)
        {
            strcpy_s(sub_codes[i], max_code_length, codes->at(i).c_str());
        }

        int ret = ut_subscribe_transactions(code_count, sub_codes);
        delete [] sub_codes;
        return ret;

    }

    //�������ί������
    int subscribe_orders(code_list* codes)
    {
        int code_count = codes->size();
        if (code_count == 0)
            return XYUT_OK;

        ut_code_string* sub_codes = new ut_code_string[code_count];//(ut_code_string*) malloc(sizeof(ut_code_string) * code_count);
        for (int i = 0; i < code_count; i++)
        {
            strcpy_s(sub_codes[i], max_code_length, codes->at(i).c_str());
        }

        int ret = ut_subscribe_orders(code_count, sub_codes);
        delete [] sub_codes;
        return ret;
    }

    //����ί�ж�������
    int subscribe_order_queues(code_list* codes)
    {
        int code_count = codes->size();
        if (code_count == 0)
            return XYUT_OK;

        ut_code_string* sub_codes = new ut_code_string[code_count];//(ut_code_string*) malloc(sizeof(ut_code_string) * code_count);
        for (int i = 0; i < code_count; i++)
        {
            strcpy_s(sub_codes[i], max_code_length, codes->at(i).c_str());
        }

        int ret = ut_subscribe_order_queues(code_count, sub_codes);
        delete [] sub_codes;
        return ret;
    }
	
    //��ÿ���
	int get_snapshot(sub_list* slist)
    {
        int code_count = slist->size();
        if (code_count == 0)
            return XYUT_OK;

        sub_item_c** items = new sub_item_c*[code_count];

        for (int i = 0; i < code_count; i++)
        {
            int fn = slist->at(i).fields.size();
            items[i] = (sub_item_c*)malloc(sizeof(sub_item_c) + (fn-1) * sizeof(sub_fields_c));
            strcpy_s(items[i]->code, max_code_length, slist->at(i).code.c_str());
            items[i]->fields.count = fn;
            memcpy(items[i]->fields.fields, &(slist->at(i).fields[0]), fn * sizeof(int));
        }

        int ret = ut_get_snapshot(code_count, items);
        
        for(int i = 0; i < code_count; i++)
        {
            free(items[i]);
        }
        delete []items;

        return ret;
    }
	
    //��ÿ���(ͬ���ӿ�)
	int get_snapshot_sync(sub_list* slist, ut_data_list& result, int timeout)
    {
        int code_count = slist->size();
        if (code_count == 0)
            return XYUT_OK;

        sub_item_c** items = new sub_item_c*[code_count];

        for (int i = 0; i < code_count; i++)
        {
            int fn = slist->at(i).fields.size();
            items[i] = (sub_item_c*)malloc(sizeof(sub_item_c) + (fn-1) * sizeof(sub_fields_c));
            strcpy_s(items[i]->code, max_code_length, slist->at(i).code.c_str());
            items[i]->fields.count = fn;
            memcpy(items[i]->fields.fields, &(slist->at(i).fields[0]), fn * sizeof(int));
        }

        ut_data_list_c* p;
        int ret = ut_get_snapshot_sync(code_count, items, &p, timeout);
        
        for(int i = 0; i < code_count; i++)
        {
            free(items[i]);
        }
        delete []items;

        if (!ret)
        {
            int n = p->code_count;
            result.related_packet_id = p->related_packet_id;
            ut_data* data = new ut_data[n];
            for (int i = 0; i < n; i++)
            {
                data[i].object_type = p->data_pointers[i]->object_type;
                data[i].xyzq_code = p->data_pointers[i]->xyzq_code;
                data[i].quotations.insert(data[i].quotations.end(), &p->data_pointers[i]->quotations[0], &p->data_pointers[i]->quotations[p->data_pointers[i]->data_length]);
            }
            result.data_list.insert(result.data_list.end(), &data[0], &data[n]);
            delete [] data;
        }

        return ret;
    }

    //��ȡ������Ϣ����δ����
	int get_baseinfo(code_list* codes)
    {
        int code_count = codes->size();
        if (code_count == 0)
            return XYUT_OK;

        ut_code_string* sub_codes = new ut_code_string[code_count];//(ut_code_string*) malloc(sizeof(ut_code_string) * code_count);
        for (int i = 0; i < code_count; i++)
        {
            strcpy_s(sub_codes[i], max_code_length, codes->at(i).c_str());
        }

        int ret = ut_get_baseinfo(code_count, sub_codes);
        delete [] sub_codes;
        return ret;
    }
	
    //�˶�
	int unsubscribe(unsub_struct *unsubscribe)
    {
        unsigned n = unsubscribe->codes.size();
        if (n == 0)
            return XYUT_OK;
        
        ut_code_string* p = new ut_code_string[n];
        for (unsigned i = 0; i < n; i++)
        {
            strcpy(p[i], unsubscribe->codes[i].c_str());
        }
        int res = ut_unsubscribe(unsubscribe->sub_type, n, p);
        delete []p;
        return res;
    }

    //ȫ���˶�
    int unsubscribe_all(int sub_type)
    {
        return ut_unsubscribe_all(sub_type);
    }

    //�����û�����
    int set_config(int config_item, int value)
    {
        return ut_set_config(config_item, value);
    }

    //��ȡ�û�����
    int get_config(int config_item, int& value)
    {
        return ut_get_config(config_item, &value);
    }
    
	//�ͷ�api
	int release() 
    {
        return ut_release();
    }
    ~xyzq_ut_api() {};

private:
    xyzq_ut_api() {};
    static xyzq_ut_spi* custom_spi(xyzq_ut_spi* new_spi = NULL) // NULL: get; non_NULL: set
    {
        static xyzq_ut_spi* m_spi = NULL;
        if (new_spi)
            m_spi = new_spi;
        return m_spi;
    }

    // �ڲ�ʹ�ã��������
    void static __stdcall virtual_callback (int msgtype, void* msg_body)
    {
        xyzq_ut_spi* spi = custom_spi();
        if (!spi)
            return;

        switch(msgtype)
        {
        case ut_error_msg:
            {
            error_msg msg;
            error_msg_c* p = (error_msg_c*)msg_body;
            msg.err_code = p->err_code;
            msg.related_packet_id = p->related_packet_id;
            msg.msg = p->msg;
            spi->rsp_error(&msg);
            }
            break;
        case ut_status_changed_msg:
            spi->rsp_status_changed(*((int*)msg_body));
            break;
        case ut_market_data_msg:
            {
                ut_data_c* p = (ut_data_c *)msg_body;
                ut_data data;
                data.object_type = p->object_type;
                data.xyzq_code = p->xyzq_code;
                data.quotations.insert(data.quotations.end(), &p->quotations[0], &p->quotations[p->data_length]);
                spi->rsp_market_data(&data);    
            }
            
            break;
        case ut_multi_market_data_msg:
            {
                ut_data_list_c* p = (ut_data_list_c *)msg_body;
                int n = p->code_count;
                ut_data_list d;
                d.related_packet_id = p->related_packet_id;
                ut_data* data = new ut_data[n];
                for (int i = 0; i < n; i++)
                {
                    data[i].object_type = p->data_pointers[i]->object_type;
                    data[i].xyzq_code = p->data_pointers[i]->xyzq_code;
                    data[i].quotations.insert(data[i].quotations.end(), &p->data_pointers[i]->quotations[0], &p->data_pointers[i]->quotations[p->data_pointers[i]->data_length]);
                }
                d.data_list.insert(d.data_list.end(), &data[0], &data[n]);
                spi->rsp_market_data_ex(&d);
                delete [] data;
            }
            
            break;
        case ut_transactions_msg:
            {
                ut_data_c* p = (ut_data_c *)msg_body;
                ut_data data;
                data.object_type = p->object_type;
                data.xyzq_code = p->xyzq_code;
                data.quotations.insert(data.quotations.end(), &p->quotations[0], &p->quotations[p->data_length]);
                spi->rsp_transactions(&data);    
            }
            break;
        case ut_orders_msg:
            {
                ut_data_c* p = (ut_data_c *)msg_body;
                ut_data data;
                data.object_type = p->object_type;
                data.xyzq_code = p->xyzq_code;
                data.quotations.insert(data.quotations.end(), &p->quotations[0], &p->quotations[p->data_length]);
                spi->rsp_orders(&data);    
            }
            break;
        case ut_order_list_msg:
            {
                ut_order_queue_list_c* p = (ut_order_queue_list_c *)msg_body;
                ut_order_queue_list data;
                data.object_type = p->object_type;
                data.xyzq_code = p->xyzq_code;
                data.millisecond = p->millisecond;
                data.time = p->time;
                data.order_queues.reserve(p->queue_count);
                for (int i = 0; i < p->queue_count; i++)
                {
                    ut_order_queue d;
                    d.direction = p->queue_pointers[i]->direction;
                    d.price = p->queue_pointers[i]->price;
                    d.orders_count = p->queue_pointers[i]->orders_count;
                    d.orders.insert(d.orders.end(), &p->queue_pointers[i]->orders[0], &p->queue_pointers[i]->orders[p->queue_pointers[i]->shown_orders_count]);
                    data.order_queues.push_back(d);
                }
                spi->rsp_order_list(&data);    
            }
            break;
        case ut_base_info_msg:
            break;
        case ut_null_msg:
        default:
            break;
        }
    }

};

#endif

