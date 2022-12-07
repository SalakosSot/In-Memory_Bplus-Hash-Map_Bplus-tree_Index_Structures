
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "BplusTree.h"

#define INPUT_DATA_SIZE 5000
#define MAXIMUM_NODE_SIZE 101
#define MINIMUM_NODE_SIZE 3
#define NODE_SIZE_INCREMENT_FACTOR 1
#define PROCESS_REPETITIONS_PER_NODE_SIZE 3

#define INPUT_DATA_RECONSTRUCTION_NUM 10



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



int main(void)
{


    BplusTree Bplustree_Index;
    DoubleLinkedBplusList Selected_Data_List;
    BplusListNode* CurrentNode = NULL;
    BplusTreeData Retrieved_Data = NULL;

    int* Input_DataSet = (int*)(malloc(INPUT_DATA_SIZE * sizeof(int)));
    int swap_position;
    int copy_data;
    int low_index_limit = 0;
    int upp_index_limit = INPUT_DATA_SIZE - 1;
    int i = 0, j = 0;
    int repetition = 0, node_size = 0, counter = 0;
    int index = 0;
    int btree_node_capacity = 0;


    int Internal_nodes_number = 0;
    int Leaf_nodes_number = 0;
    int Internal_nodes_elements = 0;
    int Leaf_nodes_elements = 0;
    int Btree_Index_height = 0;
    int unbalanced_nodes = 0;
    bool error_flag = 0;
    int duplicate_pk_values = 0;
    int improperly_structured_nodes = 0;
    bool empty_flag = 0;
    int exist_flag = 0;
    int Leaf_elements = 0;
    int internal_elements = 0;
    int leaf_only_elements = 0;
    bool execution_status = 0;



    printf("\n\n\nIntegration test - Bplus-tree index structure\n\n");
    printf("Input data size              | (%d records)\n", INPUT_DATA_SIZE);
    printf("Node size - capacity range   | min size: (%d records) - max size: (%d records)\n", MINIMUM_NODE_SIZE, MAXIMUM_NODE_SIZE);
    printf("Node size increment factor   | (%d)\n", NODE_SIZE_INCREMENT_FACTOR);
    printf("Test processes per node size | (%d)\n\n\n\n", PROCESS_REPETITIONS_PER_NODE_SIZE);



    printf("\nInput data size : (%d records)\n", INPUT_DATA_SIZE);
    printf("------------------------------------------------------------------------------------------------------------\n\n");


    for (node_size = MINIMUM_NODE_SIZE; node_size <= MAXIMUM_NODE_SIZE; node_size += NODE_SIZE_INCREMENT_FACTOR)
    {


        btree_node_capacity = node_size;

        printf("Node size - capacity: (%d records)\n", btree_node_capacity);

        InitializeBplustree(&Bplustree_Index, btree_node_capacity);
        Initialize_BplusList(&Selected_Data_List);


        for (repetition = 0; repetition < PROCESS_REPETITIONS_PER_NODE_SIZE; repetition++)
        {

            for (i = 0; i < INPUT_DATA_SIZE; i++)
            {

                Input_DataSet[i] = i;

            }


            srand(time(0));


            for (j = 0; j < INPUT_DATA_RECONSTRUCTION_NUM; j++)
            {

                for (i = 0; i < INPUT_DATA_SIZE; i++)
                {

                    swap_position = (rand() % (upp_index_limit - low_index_limit + 1)) + low_index_limit;

                    copy_data = Input_DataSet[swap_position];
                    Input_DataSet[swap_position] = Input_DataSet[i];
                    Input_DataSet[i] = copy_data;

                }

            }




            ////////////////////////////////////////////////// Insertion ///////////////////////////////////////////////////




            for (i = 0; i < INPUT_DATA_SIZE; i++)
            {

                execution_status = BplusTreeInsertData( &Bplustree_Index,
                                                       InitializeBplusTreeData_TableC(Input_DataSet[i],0),
                                                        &Get_PrimaryKey_TableC,
                                                        &Deallocate_BplusTreeRecordData_TableC );


                if (execution_status)
                {

                    printf("Test failed\n");
                    printf("Exception (0)\n\n");

                    return 1;

                }

            }




            ////////////////////////////////////////////////// Deletion ///////////////////////////////////////////////////




            srand(time(0));


            for (j = 0; j < INPUT_DATA_RECONSTRUCTION_NUM; j++)
            {

                for (i = 0; i < INPUT_DATA_SIZE; i++)
                {

                    swap_position = (rand() % (upp_index_limit - low_index_limit + 1)) + low_index_limit;

                    copy_data = Input_DataSet[swap_position];
                    Input_DataSet[swap_position] = Input_DataSet[i];
                    Input_DataSet[i] = copy_data;

                }

            }

            int deletion_fails = 0;
            int deletion_success = 0;

            for (i = 0; i < INPUT_DATA_SIZE - 1; i++)
            {

                BplusTreeDeleteData(&Bplustree_Index, Input_DataSet[i], &Retrieved_Data, &Get_PrimaryKey_TableC);


                if (Retrieved_Data != NULL)
                {

                    if (Get_PrimaryKey_TableC(Retrieved_Data) != Input_DataSet[i])
                    {

                        printf("Test failed\n");
                        printf("Exception (1)\n\n");

                        return 1;

                    }


                    Deallocate_BplusTreeRecordData_TableC(Retrieved_Data);
                    deletion_success++;


                    if (!BplusTreeDeleteData(&Bplustree_Index, Input_DataSet[i], &Retrieved_Data, &Get_PrimaryKey_TableC) || Retrieved_Data != NULL)
                    {

                        printf("Test failed\n");
                        printf("Exception (2)\n\n");

                        return 1;

                    }


                    Input_DataSet[i] = -1;

                }else{

                    deletion_fails++;

                }


                if (Bplustree_Index.size != INPUT_DATA_SIZE - deletion_success)
                {

                    printf("Test failed\n");
                    printf("Exception (3)\n\n");

                    return 1;

                }


                if (Bplustree_Index.size != INPUT_DATA_SIZE - (i + 1))
                {

                    printf("Test failed\n");
                    printf("Exception (4)\n\n");

                    return 1;

                }


                BplusTreeComputeHeight(Bplustree_Index, &Btree_Index_height);


                if (Bplustree_Index.height != Btree_Index_height)
                {

                    printf("Test failed\n");
                    printf("Exception (5)\n\n");

                    return 1;

                }


                BplusTreeCheckNodeCapacity(Bplustree_Index, &error_flag);


                if (error_flag)
                {

                    printf("Test failed\n");
                    printf("Exception (6)\n\n");

                    return 1;

                }


                BplusTreeCheckLeafLinkNodes(Bplustree_Index, &improperly_structured_nodes);


                if (improperly_structured_nodes > 0)
                {

                    printf("Test failed\n");
                    printf("Exception (7)\n\n");

                    return 1;

                }


                BplusTreeCheckBalance(Bplustree_Index, Bplustree_Index.height, &unbalanced_nodes);


                if (unbalanced_nodes > 0)
                {

                    printf("Test failed\n");
                    printf("Exception (8)\n\n");

                    return 1;

                }


                BplusTreeCheckInternalNode_ListLinks(Bplustree_Index, &improperly_structured_nodes);


                if (improperly_structured_nodes > 0)
                {

                    printf("Test failed\n");
                    printf("Exception (9)\n\n");

                    return 1;

                }


                for (j = 0; j < INPUT_DATA_SIZE; j++)
                {

                    if (Input_DataSet[j] != -1)
                    {

                        Retrieved_Data = NULL;
                        Retrieved_Data = BplusTreeFastSearchData_ByPrimaryKey(Bplustree_Index, Input_DataSet[j], &Get_PrimaryKey_TableC);


                        if (Retrieved_Data == NULL || Get_PrimaryKey_TableC(Retrieved_Data) != Input_DataSet[j])
                        {

                            printf("Test failed\n");
                            printf("Exception (10)\n\n");

                            return 1;

                        }

                    }

                }


                if (BplusTreeSelectRecordData_ASC(Bplustree_Index, &Selected_Data_List, 0, &Get_FieldData_TableC))
                {

                    printf("Test failed\n");
                    printf("Exception (11)\n\n");

                    return 1;

                }


                if (Selected_Data_List.List_Size != INPUT_DATA_SIZE - deletion_success)
                {

                    printf("Test failed\n");
                    printf("Exception (12)\n\n");

                    return 1;

                }


                if (Selected_Data_List.List_Size != INPUT_DATA_SIZE - (i + 1))
                {

                    printf("Test failed\n");
                    printf("Exception (13)\n\n");

                    return 1;

                }


                BplusTreeComputeNodes(Bplustree_Index, &Internal_nodes_number, &Internal_nodes_elements, &Leaf_nodes_number, &Leaf_nodes_elements);
                Leaf_elements = Leaf_nodes_elements - Internal_nodes_elements;


                if (Leaf_elements + Internal_nodes_elements != INPUT_DATA_SIZE - deletion_success)
                {

                    printf("Test failed\n");
                    printf("Exception (14)\n\n");

                    return 1;

                }


                if (Selected_Data_List.List_Size != Leaf_elements + Internal_nodes_elements)
                {

                    printf("Test failed\n");
                    printf("Exception (15)\n\n");

                    return 1;

                }


                if (Internal_nodes_number >= Leaf_nodes_number)
                {

                    printf("Test failed\n");
                    printf("Exception (16)\n\n");

                    return 1;

                }


                if (Internal_nodes_elements >= Leaf_nodes_elements)
                {

                    printf("Test failed\n");
                    printf("Exception (17)\n\n");

                    return 1;

                }


                if (Internal_nodes_elements + 1 != Leaf_nodes_number)
                {

                    printf("Test failed\n");
                    printf("Exception (18)\n\n");

                    return 1;

                }


                internal_elements = 0;
                leaf_only_elements = 0;


                for (j = 0; j < INPUT_DATA_SIZE; j++)
                {

                    if (Input_DataSet[j] != -1)
                    {

                        Retrieved_Data = NULL;
                        Retrieved_Data = BplusTreeSearch_NodeType_ByRecord(Bplustree_Index, Input_DataSet[j], &exist_flag, &Get_PrimaryKey_TableC);


                        if (Retrieved_Data == NULL || Get_PrimaryKey_TableC(Retrieved_Data) != Input_DataSet[j])
                        {

                            printf("Test failed\n");
                            printf("Exception (19)\n\n");

                            return 1;

                        }


                        if (exist_flag == 0 || exist_flag > 2)
                        {

                            printf("Test failed\n");
                            printf("Exception (20)\n\n");

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

                    }


                }


                if (leaf_only_elements != Leaf_elements)
                {

                    printf("Test failed\n");
                    printf("Exception (21)\n\n");

                    return 1;

                }


                if (internal_elements != Internal_nodes_elements)
                {

                    printf("Test failed\n");
                    printf("Exception (22)\n\n");

                    return 1;

                }


                if (Leaf_nodes_elements - internal_elements != Leaf_elements)
                {

                    printf("Test failed\n");
                    printf("Exception (23)\n\n");

                    return 1;

                }


                DeleteBplusList(&Selected_Data_List);

                empty_flag = BplusTreeSelectRecordData_Rec_ASC(Bplustree_Index, &Selected_Data_List);
                BplusTreeComputeNodes(Bplustree_Index, &Internal_nodes_number, &Internal_nodes_elements, &Leaf_nodes_number, &Leaf_nodes_elements);


                if (Selected_Data_List.List_Size != Internal_nodes_elements + Leaf_nodes_elements)
                {

                    printf("Test failed\n");
                    printf("Exception (24)\n\n");

                    return 1;

                }


                duplicate_pk_values = 0;


                if (!empty_flag)
                {

                    CurrentNode = Selected_Data_List.List_HeadNode->List_NextNode;


                    while (CurrentNode != NULL)
                    {

                        if (Get_PrimaryKey_TableC(CurrentNode->RecordData) < Get_PrimaryKey_TableC(CurrentNode->List_PrevNode->RecordData))
                        {

                            printf("Test failed\n");
                            printf("Exception (25)\n\n");

                            return 1;

                        }else{

                            if (Get_PrimaryKey_TableC(CurrentNode->RecordData) == Get_PrimaryKey_TableC(CurrentNode->List_PrevNode->RecordData))
                            {

                                duplicate_pk_values++;

                            }

                        }


                        CurrentNode = CurrentNode->List_NextNode;

                    }


                }else {

                    printf("Test failed\n");
                    printf("Exception (26)\n\n");

                    return 1;

                }


                if (duplicate_pk_values != Internal_nodes_elements)
                {

                    printf("Test failed\n");
                    printf("Exception (27)\n\n");

                    return 1;

                }


                DeleteBplusList(&Selected_Data_List);


                if (BplusTreeSelectRecordData_ASC(Bplustree_Index, &Selected_Data_List, 0, &Get_FieldData_TableC))
                {

                    printf("Test failed\n");
                    printf("Exception (28)\n\n");

                    return 1;

                }


                if (Selected_Data_List.List_Size != INPUT_DATA_SIZE - deletion_success)
                {

                    printf("Test failed\n");
                    printf("Exception (29)\n\n");

                    return 1;

                }


                CurrentNode = Selected_Data_List.List_HeadNode->List_NextNode;
                index = 0;

                Retrieved_Data = NULL;
                Retrieved_Data = BplusTreeFastSearchData_ByPrimaryKey( Bplustree_Index,
                                                                       Get_PrimaryKey_TableC(Selected_Data_List.List_HeadNode->RecordData),
                                                                       &Get_PrimaryKey_TableC );


                if (Retrieved_Data == NULL || Get_PrimaryKey_TableC(Retrieved_Data) != Get_PrimaryKey_TableC(Selected_Data_List.List_HeadNode->RecordData))
                {

                    printf("Test failed\n");
                    printf("Exception (30)\n\n");

                    return 1;

                }


                while (CurrentNode != NULL)
                {

                    Retrieved_Data = NULL;
                    Retrieved_Data = BplusTreeFastSearchData_ByPrimaryKey(Bplustree_Index, Get_PrimaryKey_TableC(CurrentNode->RecordData), &Get_PrimaryKey_TableC);

                    if (Retrieved_Data == NULL || Get_PrimaryKey_TableC(Retrieved_Data) != Get_PrimaryKey_TableC(CurrentNode->RecordData))
                    {

                        printf("Test failed\n");
                        printf("Exception (31)\n\n");

                        return 1;

                    }


                    if (Get_PrimaryKey_TableC(CurrentNode->List_PrevNode->RecordData) >= Get_PrimaryKey_TableC(CurrentNode->RecordData))
                    {

                        printf("Test failed\n");
                        printf("Exception (32)\n\n");

                        return 1;

                    }


                    CurrentNode = CurrentNode->List_NextNode;
                    index++;

                }


                DeleteBplusList(&Selected_Data_List);


                if (BplusTreeSelectRecordData_DESC(Bplustree_Index, &Selected_Data_List,0, &Get_FieldData_TableC))
                {

                    printf("Test failed\n");
                    printf("Exception (33)\n\n");

                    return 1;

                }


                if (Selected_Data_List.List_Size != INPUT_DATA_SIZE - deletion_success)
                {

                    printf("Test failed\n");
                    printf("Exception (34)\n\n");

                    return 1;

                }


                CurrentNode = Selected_Data_List.List_HeadNode->List_NextNode;
                index = 0;


                Retrieved_Data = NULL;
                Retrieved_Data = BplusTreeFastSearchData_ByPrimaryKey( Bplustree_Index,
                                                                       Get_PrimaryKey_TableC(Selected_Data_List.List_HeadNode->RecordData),
                                                                       &Get_PrimaryKey_TableC );


                if ( Retrieved_Data == NULL
                     || Get_PrimaryKey_TableC(Retrieved_Data) != Get_PrimaryKey_TableC(Selected_Data_List.List_HeadNode->RecordData) )
                {

                    printf("Test failed\n");
                    printf("Exception (35)\n\n");

                    return 1;

                }


                while (CurrentNode != NULL)
                {

                    Retrieved_Data = NULL;
                    Retrieved_Data = BplusTreeFastSearchData_ByPrimaryKey( Bplustree_Index,
                                                                           Get_PrimaryKey_TableC(CurrentNode->RecordData),
                                                                           &Get_PrimaryKey_TableC );


                    if (Retrieved_Data == NULL || Get_PrimaryKey_TableC(Retrieved_Data) != Get_PrimaryKey_TableC(CurrentNode->RecordData))
                    {

                        printf("Test failed\n");
                        printf("Exception (36)\n\n");

                        return 1;

                    }


                    if (Get_PrimaryKey_TableC(CurrentNode->List_PrevNode->RecordData) <= Get_PrimaryKey_TableC(CurrentNode->RecordData))
                    {

                        printf("Test failed\n");
                        printf("Exception (37)\n\n");

                        return 1;

                    }


                    CurrentNode = CurrentNode->List_NextNode;
                    index++;

                }


                DeleteBplusList(&Selected_Data_List);


            }


            BplusTreeDeleteData(&Bplustree_Index,Input_DataSet[INPUT_DATA_SIZE - 1], &Retrieved_Data, &Get_PrimaryKey_TableC);


            if (Retrieved_Data != NULL)
            {

                if (Get_PrimaryKey_TableC(Retrieved_Data) != Input_DataSet[i])
                {

                    printf("Test failed\n");
                    printf("Exception (38)\n\n");

                    return 1;

                }


                Deallocate_BplusTreeRecordData_TableC(Retrieved_Data);

            }


            if (Bplustree_Index.size != 0)
            {

                printf("Test failed\n");
                printf("Exception (39)\n\n");

                return 1;

            }


            if (Bplustree_Index.height != 0)
            {

                printf("Test failed\n");
                printf("Exception (40)\n\n");

                return 1;

            }


            if (Bplustree_Index.Root != NULL)
            {

                printf("Test failed\n");
                printf("Exception (41)\n\n");

                return 1;

            }


            if (Bplustree_Index.size != deletion_fails)
            {

                printf("Test failed\n");
                printf("Exception (42)\n\n");

                return 1;

            }

        }


        printf("(Test passed)\n\n");

    }


    free(Input_DataSet);


    printf("\n");
    printf("(Integration test passed)\n\n");


    return 0;


}

