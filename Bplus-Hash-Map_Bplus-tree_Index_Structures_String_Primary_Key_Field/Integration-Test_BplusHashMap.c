
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "BplusHashMap.h"

#define MAXIMUM_INPUT_DATA_SIZE 5000
#define MINIMUM_INPUT_DATA_SIZE 5000
#define INPUT_DATA_SIZE_INCREMENT_FACTOR 1
#define BPLUS_HASH_MAP_LEVEL_SIZE 5

#define MAXIMUM_NODE_SIZE 101
#define MINIMUM_NODE_SIZE 3
#define NODE_SIZE_INCREMENT_FACTOR 1

#define PROCESS_REPETITIONS_PER_NODE_SIZE 3

#define INPUT_DATA_RECONSTRUCTION 10



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



// Swap 2 elements.

void swap(char *x, char *y)
{

    char t = *x; *x = *y; *y = t;

}


// Reverse buffer.

char* reverse(char *buffer, int i, int j)
{

    while (i < j)
    {

        swap(&buffer[i++], &buffer[j--]);

    }

    return buffer;

}


char* itoa(int value, char* buffer, int base)
{

    if (base < 2 || base > 32)
    {

        return buffer;

    }

    int n = abs(value);
    int i = 0;

    while (n)
    {

        int r = n % base;

        if (r >= 10)
        {

            buffer[i++] = 65 + (r - 10);

        }
        else {

            buffer[i++] = 48 + r;

        }

        n = n / base;

    }

    if (i == 0)
    {

        buffer[i++] = '0';

    }

    if (value < 0 && base == 10)
    {

        buffer[i++] = '-';

    }

    buffer[i] = '\0';

    return reverse(buffer, 0, i - 1);

}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



int main(void)
{


    BplusHashMap BHashMap_Index;
    DoubleLinkedBplusList Selected_Data_List;
    BplusTreeData Retrieved_Data = NULL;
    BplusListNode* TempNode = NULL;

    int Primary_Key_Size = 15;
    BplusTreePrimaryKey Primary_Key = (BplusTreePrimaryKey)(malloc(Primary_Key_Size * sizeof(char)));


    int* Input_DataSet = (int*)(malloc(MAXIMUM_INPUT_DATA_SIZE * sizeof(int)));
    int* Copy_Input_DataSet = (int*)(malloc(MAXIMUM_INPUT_DATA_SIZE * sizeof(int)));
    int swap_position;
    int copy_data;
    int low_index_limit = 0;
    int upp_index_limit = MAXIMUM_INPUT_DATA_SIZE - 1;
    int i = 0, j = 0, n = 0;


    int input_data_size = 0;
    int node_size = 0;
    int process_repetitions = 0;
    int index = 0;
    int btree_node_capacity = 0;


    int hybrid_index_total_size = 0;
    int btree_height = 0;
    int improperly_structured_nodes;
    int unbalanced_nodes;
    int Internal_nodes_number;
    int Internal_nodes_elements;
    int Leaf_nodes_number;
    int Leaf_nodes_elements;
    int Leaf_elements;
    int internal_elements;
    int leaf_only_elements;
    bool empty_flag;
    bool error_flag;
    int exist_flag;
    bool execution_status;



    printf("\n\n\nIntegration test - BplusHashMap index structure\n\n");

    printf("Hash Map Bplus-tree Nodes        | (%d)\n", BPLUS_HASH_MAP_LEVEL_SIZE);
    printf("Input data size range            | min size: (%d records) - max size: (%d records)\n", MINIMUM_INPUT_DATA_SIZE, MAXIMUM_INPUT_DATA_SIZE);
    printf("Input data size increment factor | (%d)\n", INPUT_DATA_SIZE_INCREMENT_FACTOR);
    printf("Node size - capacity range       | min size: (%d records) - max size: (%d records)\n", MINIMUM_NODE_SIZE, MAXIMUM_NODE_SIZE);
    printf("Node size increment factor       | (%d)\n", NODE_SIZE_INCREMENT_FACTOR);
    printf("Test processes per node size     | (%d)\n\n\n\n", PROCESS_REPETITIONS_PER_NODE_SIZE);



    for (input_data_size = MINIMUM_INPUT_DATA_SIZE; input_data_size <= MAXIMUM_INPUT_DATA_SIZE; input_data_size += INPUT_DATA_SIZE_INCREMENT_FACTOR)
    {

        low_index_limit = 0;
        upp_index_limit = input_data_size - 1;


        for (i = 0; i < input_data_size; i++)
        {

            Input_DataSet[i] = i;
            Copy_Input_DataSet[i] = i;

        }


        printf("\nInput data size : (%d records)\n", input_data_size);
        printf("------------------------------------------------------------------------------------------------------------\n\n");


        for (node_size = MINIMUM_NODE_SIZE; node_size <= MAXIMUM_NODE_SIZE; node_size += NODE_SIZE_INCREMENT_FACTOR)
        {

            btree_node_capacity = node_size;


            if (btree_node_capacity < MINIMUM_NODE_SIZE)
            {

                printf("Test failed\n\n");
                printf("Exception - Invalid Bplus-tree node capacity\n\n");

                return 1;

            }


            printf("Node size - capacity: (%d records)\n", btree_node_capacity);

            InitializeBplusHashMap(&BHashMap_Index, BPLUS_HASH_MAP_LEVEL_SIZE, btree_node_capacity);


            for (process_repetitions = 0; process_repetitions < PROCESS_REPETITIONS_PER_NODE_SIZE; process_repetitions++)
            {

                Initialize_BplusList(&Selected_Data_List);

                srand(time(0));


                for (j = 0; j < INPUT_DATA_RECONSTRUCTION; j++)
                {

                    for (i = 0; i < input_data_size; i++)
                    {

                        swap_position = (rand() % (upp_index_limit - low_index_limit + 1)) + low_index_limit;

                        copy_data = Input_DataSet[swap_position];
                        Input_DataSet[swap_position] = Input_DataSet[i];
                        Input_DataSet[i] = copy_data;

                    }

                }




                ////////////////////////////////////////////////// Insertion - Selection By Primary Key ///////////////////////////////////////////////////




                for (i = 0; i < input_data_size; i++)
                {


                    itoa(Input_DataSet[i], Primary_Key, 10);

                    Retrieved_Data = NULL;
                    Retrieved_Data = InitializeBplusTreeData_TableD(Primary_Key, Primary_Key_Size, 0);

                    execution_status = BplusHashMapInsertData( &BHashMap_Index,
                                                               Retrieved_Data,
                                                              BplusTree_DJB2_Hash_PrimaryKeyStr(Get_PrimaryKey_TableD(Retrieved_Data), BPLUS_HASH_MAP_LEVEL_SIZE),
                                                               &Get_PrimaryKey_TableD,
                                                               &Deallocate_BplusTreeRecordData_TableD );


                    if (execution_status)
                    {

                        printf("Test failed\n\n");
                        printf("Insertion process failed\n");
                        printf("Exception (1)\n\n");

                        return 1;

                    }


                    Retrieved_Data = NULL;
                    Retrieved_Data = InitializeBplusTreeData_TableD(Primary_Key, Primary_Key_Size, 0);

                    execution_status = BplusHashMapInsertData( &BHashMap_Index,
                                                               Retrieved_Data,
                                                              BplusTree_DJB2_Hash_PrimaryKeyStr(Get_PrimaryKey_TableD(Retrieved_Data), BPLUS_HASH_MAP_LEVEL_SIZE),
                                                               &Get_PrimaryKey_TableD,
                                                               &Deallocate_BplusTreeRecordData_TableD );


                    if (!execution_status)
                    {

                        printf("Test failed\n\n");
                        printf("Insertion process failed\n");
                        printf("Exception (2)\n\n");

                        return 1;

                    }


                    for(n=0; n < Primary_Key_Size; n++)
                    {

                        Primary_Key[n] = ' ';

                    }


                    if (BHashMap_Index.size != i + 1)
                    {

                        printf("Test failed\n\n");
                        printf("Insertion process failed\n");
                        printf("Exception (3)\n\n");

                        return 1;


                    }


                    hybrid_index_total_size = 0;


                    for (int j = 0; j < BPLUS_HASH_MAP_LEVEL_SIZE; j++)
                    {

                        if (BHashMap_Index.BplusTreeArray[j]->Root != NULL)
                        {

                            hybrid_index_total_size += BHashMap_Index.BplusTreeArray[j]->size;
                            BplusTreeComputeHeight(*(BHashMap_Index.BplusTreeArray[j]), &btree_height);


                            if (BHashMap_Index.BplusTreeArray[j]->height != btree_height)
                            {

                                printf("Test failed\n\n");
                                printf("Insertion process failed\n");
                                printf("Exception (4)\n\n");

                            }

                        }

                    }


                    if (hybrid_index_total_size != i + 1)
                    {

                        printf("Test failed\n\n");
                        printf("Insertion process failed\n");
                        printf("Exception (5)\n\n");

                        return 1;

                    }


                    hybrid_index_total_size = 0;


                    for (int j = 0; j <  BPLUS_HASH_MAP_LEVEL_SIZE; j++)
                    {

                        if (BHashMap_Index.BplusTreeArray[j]->Root != NULL)
                        {

                            empty_flag = BplusTreeSelectRecordData_ASC(*(BHashMap_Index.BplusTreeArray[j]), &Selected_Data_List, 0, &Get_FieldData_TableD);
                            TempNode = Selected_Data_List.List_HeadNode->List_NextNode;

                            hybrid_index_total_size += Selected_Data_List.List_Size;


                            if (!empty_flag)
                            {

                                while (TempNode != NULL)
                                {

                                    if ( strcmp(Get_PrimaryKey_TableD(TempNode->RecordData), Get_PrimaryKey_TableD(TempNode->List_PrevNode->RecordData)) <= 0 )
                                    {

                                        printf("Test failed\n\n");
                                        printf("Insertion process failed\n");
                                        printf("Exception (6)\n\n");

                                        return 1;

                                    }


                                    TempNode = TempNode->List_NextNode;

                                }

                            }


                            DeleteBplusList(&Selected_Data_List);

                        }

                    }


                    if (hybrid_index_total_size != i + 1)
                    {

                        printf("Test failed\n\n");
                        printf("Insertion process failed\n");
                        printf("Exception (7)\n\n");

                        return 1;

                    }


                    hybrid_index_total_size = 0;


                    for (int j = 0; j < BPLUS_HASH_MAP_LEVEL_SIZE; j++)
                    {

                        if (BHashMap_Index.BplusTreeArray[j]->Root != NULL)
                        {

                            empty_flag = BplusTreeSelectRecordData_DESC(*(BHashMap_Index.BplusTreeArray[j]), &Selected_Data_List,0, &Get_FieldData_TableD);
                            TempNode = Selected_Data_List.List_HeadNode->List_NextNode;

                            hybrid_index_total_size += Selected_Data_List.List_Size;


                            if (!empty_flag)
                            {

                                while (TempNode != NULL)
                                {

                                    if ( strcmp(Get_PrimaryKey_TableD(TempNode->RecordData), Get_PrimaryKey_TableD(TempNode->List_PrevNode->RecordData)) >= 0 )
                                    {

                                        printf("Test failed\n\n");
                                        printf("Insertion process failed\n");
                                        printf("Exception (8)\n\n");

                                        return 1;

                                    }


                                    TempNode = TempNode->List_NextNode;

                                }

                            }


                            DeleteBplusList(&Selected_Data_List);

                        }

                    }


                    if (hybrid_index_total_size != i + 1)
                    {

                        printf("Test failed\n\n");
                        printf("Insertion process failed\n");
                        printf("Exception (9)\n\n");

                        return 1;

                    }

                }


                hybrid_index_total_size = 0;


                for (int i = 0; i < BPLUS_HASH_MAP_LEVEL_SIZE; i++)
                {

                    if (BHashMap_Index.BplusTreeArray[i]->Root != NULL)
                    {

                        BplusTreeCheckLeafLinkNodes(*(BHashMap_Index.BplusTreeArray[i]), &improperly_structured_nodes);

                        if (improperly_structured_nodes > 0)
                        {

                            printf("Test failed\n\n");
                            printf("Insertion process failed\n");
                            printf("Exception (10)\n\n");

                            return 1;

                        }


                        BplusTreeCheckBalance(*(BHashMap_Index.BplusTreeArray[i]), BHashMap_Index.BplusTreeArray[i]->height, &unbalanced_nodes);


                        if (unbalanced_nodes > 0)
                        {

                            printf("Test failed\n\n");
                            printf("Insertion process failed\n");
                            printf("Exception (11)\n\n");

                            return 1;

                        }


                        BplusTreeCheckNodeCapacity(*(BHashMap_Index.BplusTreeArray[i]), &error_flag);


                        if (error_flag)
                        {

                            printf("Test failed\n\n");
                            printf("Insertion process failed\n");
                            printf("Exception (12)\n\n");

                            return 1;

                        }


                        BplusTreeCheckInternalNode_ListLinks(*(BHashMap_Index.BplusTreeArray[i]), &improperly_structured_nodes);


                        if (improperly_structured_nodes > 0)
                        {

                            printf("Test failed\n\n");
                            printf("Insertion process failed\n");
                            printf("Exception (13)\n\n");

                            return 1;

                        }


                        BplusTreeComputeNodes( *(BHashMap_Index.BplusTreeArray[i]),
                                               &Internal_nodes_number,
                                               &Internal_nodes_elements,
                                               &Leaf_nodes_number,
                                               &Leaf_nodes_elements );

                        Leaf_elements = Leaf_nodes_elements - Internal_nodes_elements;
                        hybrid_index_total_size += Leaf_elements + Internal_nodes_elements;


                        if (Leaf_elements + Internal_nodes_elements != BHashMap_Index.BplusTreeArray[i]->size)
                        {

                            printf("Test failed\n\n");
                            printf("Insertion process failed\n");
                            printf("Exception (14)\n\n");

                            return 1;

                        }


                        if (Internal_nodes_number >= Leaf_nodes_number)
                        {

                            printf("Test failed\n\n");
                            printf("Insertion process failed\n");
                            printf("Exception (15)\n\n");

                            return 1;

                        }


                        if (Internal_nodes_elements >= Leaf_nodes_elements)
                        {

                            printf("Test failed\n\n");
                            printf("Insertion process failed\n");
                            printf("Exception (16)\n\n");

                            return 1;

                        }


                        if (Internal_nodes_elements + 1 != Leaf_nodes_number)
                        {

                            printf("Test failed\n\n");
                            printf("Insertion process failed\n");
                            printf("Exception (17)\n\n");

                            return 1;

                        }

                    }

                }


                if (hybrid_index_total_size != input_data_size)
                {

                    printf("Test failed\n\n");
                    printf("Insertion process failed\n");
                    printf("Exception (18)\n\n");

                    return 1;

                }


                for (int j = 0; j < BPLUS_HASH_MAP_LEVEL_SIZE; j++)
                {

                    internal_elements = 0;
                    leaf_only_elements = 0;


                    if (BHashMap_Index.BplusTreeArray[j]->Root != NULL)
                    {

                        empty_flag = BplusTreeSelectRecordData_ASC(*(BHashMap_Index.BplusTreeArray[j]), &Selected_Data_List, 0, &Get_FieldData_TableD);
                        TempNode = Selected_Data_List.List_HeadNode;

                        BplusTreeComputeNodes( *(BHashMap_Index.BplusTreeArray[j]),
                                               &Internal_nodes_number,
                                               &Internal_nodes_elements,
                                               &Leaf_nodes_number,
                                               &Leaf_nodes_elements );

                        Leaf_elements = Leaf_nodes_elements - Internal_nodes_elements;


                        if (!empty_flag)
                        {

                            if (Selected_Data_List.List_Size != BHashMap_Index.BplusTreeArray[j]->size)
                            {

                                printf("Test failed\n\n");
                                printf("Insertion process failed\n");
                                printf("Exception (19)\n\n");

                                return 1;

                            }


                            while (TempNode != NULL)
                            {

                                Retrieved_Data = NULL;
                                Retrieved_Data = BplusTreeSearch_NodeType_ByRecord( *(BHashMap_Index.BplusTreeArray[j]),
                                                                                    Get_PrimaryKey_TableD(TempNode->RecordData),
                                                                                    &exist_flag,
                                                                                    &Get_PrimaryKey_TableD );


                                if ( Retrieved_Data == NULL
                                     || strcmp(Get_PrimaryKey_TableD(Retrieved_Data), Get_PrimaryKey_TableD(TempNode->RecordData)) != 0 )
                                {

                                    printf("Test failed\n\n");
                                    printf("Insertion process failed\n");
                                    printf("Exception (20)\n\n");

                                    return 1;

                                }


                                if (exist_flag == 0 || exist_flag > 2)
                                {

                                    printf("Test failed\n\n");
                                    printf("Insertion process failed\n");
                                    printf("Exception (21)\n\n");

                                    return 1;

                                }


                                if (exist_flag == 2)
                                {

                                    internal_elements++;

                                }


                                if (exist_flag == 1)
                                {

                                    leaf_only_elements++;

                                }


                                TempNode = TempNode->List_NextNode;

                            }


                            if (leaf_only_elements != Leaf_elements)
                            {

                                printf("Test failed\n\n");
                                printf("Insertion process failed\n");
                                printf("Exception (22)\n\n");

                                return 1;

                            }


                            if (internal_elements != Internal_nodes_elements)
                            {

                                printf("Test failed\n\n");
                                printf("Insertion process failed\n");
                                printf("Exception (23)\n\n");

                                return 1;

                            }


                            if (Leaf_nodes_elements - internal_elements != Leaf_elements)
                            {

                                printf("Test failed\n\n");
                                printf("Insertion process failed\n");
                                printf("Exception (24)\n\n");

                                return 1;

                            }

                        }


                        DeleteBplusList(&Selected_Data_List);

                    }

                }


                for (i = 0; i < input_data_size; i++)
                {


                    itoa(Input_DataSet[i], Primary_Key, 10);

                    Retrieved_Data = NULL;
                    Retrieved_Data = BplusHashMapSelectData_ByPrimaryKey( BHashMap_Index,
                                                                          Primary_Key,
                                                                         BplusTree_DJB2_Hash_PrimaryKeyStr(Primary_Key, BPLUS_HASH_MAP_LEVEL_SIZE),
                                                                          &Get_PrimaryKey_TableD );


                    if (Retrieved_Data == NULL || strcmp(Get_PrimaryKey_TableD(Retrieved_Data), Primary_Key) != 0 )
                    {

                        printf("Test failed\n\n");
                        printf("Insertion process failed\n");
                        printf("Exception (25)\n\n");

                        return 1;

                    }


                    for(n=0; n < Primary_Key_Size; n++)
                    {

                        Primary_Key[n] = ' ';

                    }

                }




                ////////////////////////////////////////////////// Full Selection //////////////////////////////////////////////




                empty_flag = BplusHashMapSelectData(BHashMap_Index, &Selected_Data_List,0,0, &Get_FieldData_TableD);


                if (empty_flag || Selected_Data_List.List_Size != input_data_size)
                {

                    printf("Test failed\n\n");
                    printf("Selection process failed\n");
                    printf("Exception (1)\n\n");

                    return 1;

                }


                TempNode = Selected_Data_List.List_HeadNode;


                if (!empty_flag)
                {

                    while (TempNode != NULL)
                    {

                        Retrieved_Data = NULL;
                        Retrieved_Data = BplusHashMapSelectData_ByPrimaryKey( BHashMap_Index,
                                                                              Get_PrimaryKey_TableD(TempNode->RecordData),
                                                                              BplusTree_DJB2_Hash_PrimaryKeyStr(Get_PrimaryKey_TableD(TempNode->RecordData),BPLUS_HASH_MAP_LEVEL_SIZE),
                                                                              &Get_PrimaryKey_TableD );

                        if ( Retrieved_Data == NULL
                             || strcmp(Get_PrimaryKey_TableD(Retrieved_Data), Get_PrimaryKey_TableD(TempNode->RecordData)) != 0 )
                        {

                            printf("Test failed\n\n");
                            printf("Selection process failed\n");
                            printf("Exception (2)\n\n");

                            return 1;

                        }


                        TempNode = TempNode->List_NextNode;

                    }


                }


                DeleteBplusList(&Selected_Data_List);




                ////////////////////////////////////////////////// Deletion //////////////////////////////////////////////




                for (j = 0; j < INPUT_DATA_RECONSTRUCTION; j++)
                {

                    for (i = 0; i < input_data_size; i++)
                    {

                        swap_position = (rand() % (upp_index_limit - low_index_limit + 1)) + low_index_limit;

                        copy_data = Input_DataSet[swap_position];
                        Input_DataSet[swap_position] = Input_DataSet[i];
                        Input_DataSet[i] = copy_data;

                    }

                }


                for (i = 0; i < (input_data_size - 1); i++)
                {


                    itoa(Input_DataSet[i],Primary_Key,10);

                    execution_status = BplusHashMapDeleteData( &BHashMap_Index,
                                                               Primary_Key,
                                                              BplusTree_DJB2_Hash_PrimaryKeyStr(Primary_Key, BPLUS_HASH_MAP_LEVEL_SIZE),
                                                               &Retrieved_Data,
                                                               &Get_PrimaryKey_TableD );


                    if (execution_status)
                    {

                        printf("Test failed\n\n");
                        printf("Deletion process failed\n");
                        printf("Exception (1)\n\n");

                        return 1;

                    } else {

                        Deallocate_BplusTreeRecordData_TableD(Retrieved_Data);

                    }


                    execution_status = BplusHashMapDeleteData( &BHashMap_Index,
                                                               Primary_Key,
                                                              BplusTree_DJB2_Hash_PrimaryKeyStr(Primary_Key,BPLUS_HASH_MAP_LEVEL_SIZE),
                                                               &Retrieved_Data,
                                                               &Get_PrimaryKey_TableD );


                    if (!execution_status)
                    {

                        printf("Test failed\n\n");
                        printf("Deletion process failed\n");
                        printf("Exception (2)\n\n");

                        return 1;

                    }


                    for(n=0; n < Primary_Key_Size; n++)
                    {

                        Primary_Key[n] = ' ';

                    }


                    if (BHashMap_Index.size != input_data_size - (i + 1))
                    {

                        printf("Test failed\n\n");
                        printf("Deletion process failed\n");
                        printf("Exception (3)\n\n");

                        return 1;


                    }


                    hybrid_index_total_size = 0;


                    for (int j = 0; j < BPLUS_HASH_MAP_LEVEL_SIZE; j++)
                    {

                        if (BHashMap_Index.BplusTreeArray[j]->Root != NULL)
                        {

                            hybrid_index_total_size += BHashMap_Index.BplusTreeArray[j]->size;
                            BplusTreeComputeHeight(*(BHashMap_Index.BplusTreeArray[j]), &btree_height);


                            if (BHashMap_Index.BplusTreeArray[j]->height != btree_height)
                            {

                                printf("Test failed\n\n");
                                printf("Deletion process failed\n");
                                printf("Exception (4)\n\n");

                            }

                        }

                    }


                    if (hybrid_index_total_size != input_data_size - (i + 1))
                    {

                        printf("Test failed\n\n");
                        printf("Deletion process failed\n");
                        printf("Exception (5)\n\n");

                        return 1;

                    }


                    hybrid_index_total_size = 0;


                    for (int j = 0; j < BPLUS_HASH_MAP_LEVEL_SIZE; j++)
                    {

                        if (BHashMap_Index.BplusTreeArray[j]->Root != NULL)
                        {

                            empty_flag = BplusTreeSelectRecordData_ASC(*(BHashMap_Index.BplusTreeArray[j]), &Selected_Data_List, 0, &Get_FieldData_TableD);
                            TempNode = Selected_Data_List.List_HeadNode->List_NextNode;

                            hybrid_index_total_size += Selected_Data_List.List_Size;


                            if (!empty_flag)
                            {

                                while (TempNode != NULL)
                                {

                                    if ( strcmp(Get_PrimaryKey_TableD(TempNode->RecordData), Get_PrimaryKey_TableD(TempNode->List_PrevNode->RecordData)) <= 0 )
                                    {

                                        printf("Test failed\n\n");
                                        printf("Deletion process failed\n");
                                        printf("Exception (6)\n\n");

                                        return 1;

                                    }


                                    TempNode = TempNode->List_NextNode;

                                }

                            }


                            DeleteBplusList(&Selected_Data_List);

                        }

                    }


                    if (hybrid_index_total_size != input_data_size - (i + 1))
                    {

                        printf("Test failed\n\n");
                        printf("Deletion process failed\n");
                        printf("Exception (7)\n\n");

                        return 1;

                    }


                    hybrid_index_total_size = 0;


                    for (int j = 0; j < BPLUS_HASH_MAP_LEVEL_SIZE; j++)
                    {

                        if (BHashMap_Index.BplusTreeArray[j]->Root != NULL)
                        {

                            empty_flag = BplusTreeSelectRecordData_DESC(*(BHashMap_Index.BplusTreeArray[j]), &Selected_Data_List, 0, &Get_FieldData_TableD);
                            TempNode = Selected_Data_List.List_HeadNode->List_NextNode;

                            hybrid_index_total_size += Selected_Data_List.List_Size;


                            if (!empty_flag)
                            {

                                while (TempNode != NULL)
                                {

                                    if ( strcmp(Get_PrimaryKey_TableD(TempNode->RecordData), Get_PrimaryKey_TableD(TempNode->List_PrevNode->RecordData)) >= 0 )
                                    {

                                        printf("Test failed\n\n");
                                        printf("Deletion process failed\n");
                                        printf("Exception (8)\n\n");

                                        return 1;

                                    }


                                    TempNode = TempNode->List_NextNode;

                                }

                            }


                            DeleteBplusList(&Selected_Data_List);

                        }

                    }


                    if (hybrid_index_total_size != input_data_size - (i + 1))
                    {

                        printf("Test failed\n\n");
                        printf("Deletion process failed\n");
                        printf("Exception (9)\n\n");

                        return 1;

                    }


                    hybrid_index_total_size = 0;


                    for (int j = 0; j < BPLUS_HASH_MAP_LEVEL_SIZE; j++)
                    {

                        if (BHashMap_Index.BplusTreeArray[j]->Root != NULL)
                        {

                            BplusTreeCheckLeafLinkNodes(*(BHashMap_Index.BplusTreeArray[j]), &improperly_structured_nodes);

                            if (improperly_structured_nodes > 0)
                            {

                                printf("Test failed\n\n");
                                printf("Deletion process failed\n");
                                printf("Exception (10)\n\n");

                                return 1;

                            }


                            BplusTreeCheckBalance(*(BHashMap_Index.BplusTreeArray[j]), BHashMap_Index.BplusTreeArray[j]->height, &unbalanced_nodes);


                            if (unbalanced_nodes > 0)
                            {

                                printf("Test failed\n\n");
                                printf("Deletion process failed\n");
                                printf("Exception (11)\n\n");

                                return 1;

                            }


                            BplusTreeCheckNodeCapacity(*(BHashMap_Index.BplusTreeArray[j]), &error_flag);


                            if (error_flag)
                            {

                                printf("Test failed\n\n");
                                printf("Deletion process failed\n");
                                printf("Exception (12)\n\n");

                                return 1;

                            }


                            BplusTreeCheckInternalNode_ListLinks(*(BHashMap_Index.BplusTreeArray[j]), &improperly_structured_nodes);


                            if (improperly_structured_nodes > 0)
                            {

                                printf("Test failed\n\n");
                                printf("Deletion process failed\n");
                                printf("Exception (13)\n\n");

                                return 1;

                            }


                            BplusTreeComputeNodes( *(BHashMap_Index.BplusTreeArray[j]),
                                                   &Internal_nodes_number,
                                                   &Internal_nodes_elements,
                                                   &Leaf_nodes_number,
                                                   &Leaf_nodes_elements );

                            Leaf_elements = Leaf_nodes_elements - Internal_nodes_elements;
                            hybrid_index_total_size += Leaf_elements + Internal_nodes_elements;


                            if (Leaf_elements + Internal_nodes_elements != BHashMap_Index.BplusTreeArray[j]->size)
                            {

                                printf("Test failed\n\n");
                                printf("Deletion process failed\n");
                                printf("Exception (14)\n\n");

                                return 1;

                            }


                            if (Internal_nodes_number >= Leaf_nodes_number)
                            {

                                printf("Test failed\n\n");
                                printf("Deletion process failed\n");
                                printf("Exception (15)\n\n");

                                return 1;

                            }


                            if (Internal_nodes_elements >= Leaf_nodes_elements)
                            {

                                printf("Test failed\n\n");
                                printf("Deletion process failed\n");
                                printf("Exception (16)\n\n");

                                return 1;

                            }


                            if (Internal_nodes_elements + 1 != Leaf_nodes_number)
                            {

                                printf("Test failed\n\n");
                                printf("Deletion process failed\n");
                                printf("Exception (17)\n\n");

                                return 1;

                            }

                        }

                    }


                    if (hybrid_index_total_size != input_data_size - (i + 1))
                    {

                        printf("Test failed\n\n");
                        printf("Deletion process failed\n");
                        printf("Exception (18)\n\n");

                        return 1;

                    }


                    for (int j = 0; j < BPLUS_HASH_MAP_LEVEL_SIZE; j++)
                    {

                        internal_elements = 0;
                        leaf_only_elements = 0;


                        if (BHashMap_Index.BplusTreeArray[j]->Root != NULL)
                        {

                            empty_flag = BplusTreeSelectRecordData_ASC(*(BHashMap_Index.BplusTreeArray[j]), &Selected_Data_List, 0, &Get_FieldData_TableD);
                            TempNode = Selected_Data_List.List_HeadNode;


                            BplusTreeComputeNodes( *(BHashMap_Index.BplusTreeArray[j]),
                                                   &Internal_nodes_number,
                                                   &Internal_nodes_elements,
                                                   &Leaf_nodes_number,
                                                   &Leaf_nodes_elements );

                            Leaf_elements = Leaf_nodes_elements - Internal_nodes_elements;


                            if (!empty_flag)
                            {

                                if (Selected_Data_List.List_Size != BHashMap_Index.BplusTreeArray[j]->size)
                                {

                                    printf("Test failed\n\n");
                                    printf("Deletion process failed\n");
                                    printf("Exception (19)\n\n");

                                    return 1;

                                }


                                while (TempNode != NULL)
                                {

                                    Retrieved_Data = NULL;
                                    Retrieved_Data = BplusTreeSearch_NodeType_ByRecord( *(BHashMap_Index.BplusTreeArray[j]),
                                                                                        Get_PrimaryKey_TableD(TempNode->RecordData),
                                                                                        &exist_flag,
                                                                                        &Get_PrimaryKey_TableD );


                                    if ( Retrieved_Data == NULL
                                         || strcmp(Get_PrimaryKey_TableD(Retrieved_Data), Get_PrimaryKey_TableD(TempNode->RecordData)) != 0 )
                                    {

                                        printf("Test failed\n\n");
                                        printf("Deletion process failed\n");
                                        printf("Exception (20)\n\n");

                                        return 1;

                                    }


                                    if (exist_flag == 0 || exist_flag > 2)
                                    {

                                        printf("Test failed\n\n");
                                        printf("Deletion process failed\n");
                                        printf("Exception (21)\n\n");

                                        return 1;

                                    }


                                    if (exist_flag == 2)
                                    {

                                        internal_elements++;

                                    }


                                    if (exist_flag == 1)
                                    {

                                        leaf_only_elements++;

                                    }


                                    TempNode = TempNode->List_NextNode;

                                }


                                if (leaf_only_elements != Leaf_elements)
                                {

                                    printf("Test failed\n\n");
                                    printf("Deletion process failed\n");
                                    printf("Exception (22)\n\n");

                                    return 1;

                                }


                                if (internal_elements != Internal_nodes_elements)
                                {

                                    printf("Test failed\n\n");
                                    printf("Deletion process failed\n");
                                    printf("Exception (23)\n\n");

                                    return 1;

                                }


                                if (Leaf_nodes_elements - internal_elements != Leaf_elements)
                                {

                                    printf("Test failed\n\n");
                                    printf("Deletion process failed\n");
                                    printf("Exception (24)\n\n");

                                    return 1;

                                }

                            }


                            DeleteBplusList(&Selected_Data_List);

                        }

                    }


                    for (int j = 0; j < BPLUS_HASH_MAP_LEVEL_SIZE; j++)
                    {

                        if (BHashMap_Index.BplusTreeArray[j]->Root != NULL)
                        {

                            empty_flag = BplusTreeSelectRecordData_ASC(*(BHashMap_Index.BplusTreeArray[j]), &Selected_Data_List,0, &Get_FieldData_TableD);
                            TempNode = Selected_Data_List.List_HeadNode;


                            if (Selected_Data_List.List_Size != BHashMap_Index.BplusTreeArray[j]->size)
                            {

                                printf("Test failed\n\n");
                                printf("Deletion process failed\n");
                                printf("Exception (25)\n\n");

                                return 1;

                            }


                            if (!empty_flag)
                            {

                                while (TempNode != NULL)
                                {

                                    Retrieved_Data = NULL;
                                    Retrieved_Data = BplusHashMapSelectData_ByPrimaryKey( BHashMap_Index,
                                                                                          Get_PrimaryKey_TableD(TempNode->RecordData),
                                                                                          BplusTree_DJB2_Hash_PrimaryKeyStr(Get_PrimaryKey_TableD(TempNode->RecordData), BPLUS_HASH_MAP_LEVEL_SIZE),
                                                                                          &Get_PrimaryKey_TableD );

                                    if ( Retrieved_Data == NULL
                                         || strcmp(Get_PrimaryKey_TableD(Retrieved_Data), Get_PrimaryKey_TableD(TempNode->RecordData)) != 0 )
                                    {

                                        printf("Test failed\n\n");
                                        printf("Deletion process failed\n");
                                        printf("Exception (26)\n\n");
                                        return 1;

                                    }


                                    TempNode = TempNode->List_NextNode;

                                }

                            }


                            DeleteBplusList(&Selected_Data_List);

                        }

                    }

                }


                itoa(Input_DataSet[input_data_size - 1], Primary_Key, 10);

                execution_status = BplusHashMapDeleteData( &BHashMap_Index,
                                                           Primary_Key,
                                                          BplusTree_DJB2_Hash_PrimaryKeyStr(Primary_Key,BPLUS_HASH_MAP_LEVEL_SIZE),
                                                           &Retrieved_Data,
                                                           &Get_PrimaryKey_TableD );


                if (execution_status)
                {

                    printf("Test failed\n\n");
                    printf("Deletion process failed\n");
                    printf("Exception (27)\n\n");

                    return 1;

                }else {

                    Deallocate_BplusTreeRecordData_TableD(Retrieved_Data);

                }


                for(n=0; n < Primary_Key_Size; n++)
                {

                    Primary_Key[n] = ' ';

                }


                if (BHashMap_Index.size != 0)
                {

                    printf("Test failed\n\n");
                    printf("Deletion process failed\n");
                    printf("Exception (28)\n\n");

                    return 1;

                }


                if (!BplusHashMapClean(&BHashMap_Index, &Deallocate_BplusTreeRecordData_TableD))
                {

                    printf("Test failed\n\n");
                    printf("Deletion process failed\n");
                    printf("Exception (29)\n\n");

                    return 1;

                }

            }


            BplusHashMapDrop(&BHashMap_Index, &Deallocate_BplusTreeRecordData_TableD);

            printf("(Test passed)\n\n");

        }

    }


    free(Input_DataSet);
    free(Copy_Input_DataSet);
    free(Primary_Key);


    printf("\n");
    printf("(Integration test passed)\n\n");



    return 0;


}

