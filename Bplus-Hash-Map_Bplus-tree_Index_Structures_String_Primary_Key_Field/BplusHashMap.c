
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "BplusHashMap.h"



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void InitializeBplusHashMap(

        BplusHashMap* BplusHashMap,
        int BplusHashMap_Level_Size,
        int BplusTreeNodeSize

)
{


    BplusHashMap->BplusHashMap_Level_Size = BplusHashMap_Level_Size;
    BplusHashMap->BplusTreeArray = (BplusTree**)(malloc(BplusHashMap->BplusHashMap_Level_Size * sizeof(BplusTree*)));


    for (int i = 0; i < BplusHashMap->BplusHashMap_Level_Size; i++)
    {

        BplusHashMap->BplusTreeArray[i] = (BplusTree*)(malloc(sizeof(BplusTree)));
        InitializeBplustree(BplusHashMap->BplusTreeArray[i], BplusTreeNodeSize);

    }

    BplusHashMap->size = 0;


    return;


}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool BplusHashMapInsertData(

        BplusHashMap* BplusHashMap,
        BplusHashMapData RecordData,
        int hash_code,

        BplusTreePrimaryKey (*Get_Record_PrimaryKey)(BplusTreeData),
        bool (*Deallocate_Record_Data)(BplusTreeData)

)
{


    if (BplusHashMap->BplusTreeArray == NULL)
    {

        printf("BplusHashMap is uninitialized\n\n");

        return 1;

    }


    if (!BplusTreeInsertData(BplusHashMap->BplusTreeArray[hash_code], RecordData, Get_Record_PrimaryKey, Deallocate_Record_Data))
    {

        BplusHashMap->size += 1;

        return 0;

    }


    return 1;


}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool BplusHashMapDeleteData(

        BplusHashMap* BplusHashMap,
        BplusTreePrimaryKey PrimaryKey,
        int hash_code,
        BplusHashMapData* DeletedRecordData,

        BplusTreePrimaryKey (*Get_Record_PrimaryKey)(BplusTreeData)

)
{


    if (BplusHashMap->BplusTreeArray == NULL)
    {

        //printf("BplusHashMap is uninitialized\n\n");

        *DeletedRecordData = NULL;

        return 1;

    }


    if (BplusHashMap->size == 0)
    {

        //printf("BplusHashMap is empty\n\n");

        *DeletedRecordData = NULL;

        return 1;

    }


    if (BplusHashMap->BplusTreeArray[hash_code]->Root == NULL)
    {

        //printf("BplusHashMap Bplus-tree is empty\n\n");

        *DeletedRecordData = NULL;

        return 1;

    }


    if (!BplusTreeDeleteData(BplusHashMap->BplusTreeArray[hash_code], PrimaryKey, DeletedRecordData, Get_Record_PrimaryKey))
    {

        BplusHashMap->size -= 1;

        return 0;

    }


    return 1;


}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BplusHashMapDisplay(

        BplusHashMap BplusHashMap,

        BplusTreePrimaryKey (*Get_Record_PrimaryKey)(BplusTreeData)

)
{


    if (BplusHashMap.BplusTreeArray == NULL)
    {

        //printf("BplusHashMap is uninitialized\n\n");

        return;

    }


    if (BplusHashMap.size == 0)
    {

        //printf("BplusHashMap is empty\n\n");

        return;

    }


    printf("------------------------- Hash Map -------------------------\n\n");


    for (int i = 0; i < BplusHashMap.BplusHashMap_Level_Size; i++)
    {

        BplusTreeDisplayInOrder(*(BplusHashMap.BplusTreeArray[i]), Get_Record_PrimaryKey);

        printf("\n");

    }


    printf("\n------------------------------------------------------------\n\n");


    return;


}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool BplusHashMapSelectData(

        BplusHashMap BplusHashMap,
        struct DoubleLinkedBplusList* SelectionResultList,
        int Field_Data,
        bool selection_method_flag,

        int (*Get_Record_FieldData)(BplusTreeData)

)
{


    if (BplusHashMap.BplusTreeArray == NULL)
    {

        //printf("BplusHashMap is uninitialized\n\n");

        return 1;

    }


    if (BplusHashMap.size == 0)
    {

        //printf("BplusHashMap is empty\n\n");

        return 1;

    }


    for (int i = 0; i < BplusHashMap.BplusHashMap_Level_Size; i++)
    {

        if (BplusHashMap.BplusTreeArray[i]->Root != NULL)
        {

            if (!selection_method_flag)
            {

                BplusTreeSelectRecordData_ASC(*(BplusHashMap.BplusTreeArray[i]), SelectionResultList, Field_Data, Get_Record_FieldData);

            }else{

                BplusTreeSelectRecordData_DESC(*(BplusHashMap.BplusTreeArray[i]), SelectionResultList, Field_Data, Get_Record_FieldData);

            }

        }

    }


    if (SelectionResultList->List_HeadNode != NULL)
    {

        return 0;

    }


    return 1;


}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BplusHashMapData BplusHashMapSelectData_ByPrimaryKey(

        BplusHashMap BplusHashMap,
        BplusTreePrimaryKey PrimaryKey,
        int hash_code,

        BplusTreePrimaryKey (*Get_Record_PrimaryKey)(BplusTreeData)

)
{


    if (BplusHashMap.BplusTreeArray == NULL)
    {

        //printf("BplusHashMap is uninitialized\n\n");

        return NULL;

    }


    if (BplusHashMap.size == 0)
    {

        //printf("BplusHashMap is empty\n\n");

        return NULL;

    }


    if (BplusHashMap.BplusTreeArray[hash_code]->Root == NULL)
    {

        //printf("BplusHashMap Bplus-tree is empty - Data not found\n\n");

        return NULL;

    }


    return BplusTreeFastSearchData_ByPrimaryKey(*(BplusHashMap.BplusTreeArray[hash_code]), PrimaryKey, Get_Record_PrimaryKey);


}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool BplusHashMapDrop(

        BplusHashMap* BplusHashMap,

        bool (*Deallocate_Record_Data)(BplusTreeData)

)
{


    if (BplusHashMap->BplusTreeArray == NULL)
    {

        //printf("BplusHashMap is uninitialized\n\n");

        return 1;

    }


    for (int i = 0; i < BplusHashMap->BplusHashMap_Level_Size; i++)
    {

        if (BplusHashMap->BplusTreeArray[i]->Root != NULL)
        {

            BplusTreeDrop(BplusHashMap->BplusTreeArray[i], Deallocate_Record_Data);

        }


        free(BplusHashMap->BplusTreeArray[i]);

    }


    free(BplusHashMap->BplusTreeArray);


    BplusHashMap->BplusTreeArray = NULL;
    BplusHashMap->size = 0;


    return 0;


}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool BplusHashMapClean(

        BplusHashMap* BplusHashMap,

        bool (*Deallocate_Record_Data)(BplusTreeData)

)
{


    if (BplusHashMap->BplusTreeArray == NULL)
    {

        //printf("BplusHashMap is uninitialized\n\n");

        return 1;

    }


    if (BplusHashMap->size == 0)
    {

        //printf("BplusHashMap is empty\n\n");

        return 1;

    }


    for (int i = 0; i < BplusHashMap->BplusHashMap_Level_Size; i++)
    {

        if (BplusHashMap->BplusTreeArray[i]->Root != NULL)
        {

            BplusTreeDrop(BplusHashMap->BplusTreeArray[i], Deallocate_Record_Data);

        }

    }


    BplusHashMap->size = 0;


    return 0;


}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int BplusTree_DJB2_Hash_PrimaryKeyStr(

        char* record_primary_key,
        int hashmap_level_size

)
{


    long hash_code = 5381;
    int c;


    while (c = *record_primary_key++)
    {

        hash_code = ((hash_code << 5) + hash_code) + c; /* hash * 33 + c */

    }


    if (hash_code < 0)
    {

        hash_code *= -1;

    }


    return hash_code % hashmap_level_size;


}

