
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
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


    BplusTree Bplustree_Index;
    DoubleLinkedBplusList Selected_Data_List;
    BplusListNode* CurrentNode = NULL;
    BplusTreeData Retrieved_Data = NULL;

    int Primary_Key_Size = 15;
    BplusTreePrimaryKey Primary_Key = (BplusTreePrimaryKey)(malloc(Primary_Key_Size * sizeof(char)));


    int* Input_DataSet = (int*)(malloc(INPUT_DATA_SIZE * sizeof(int)));
    int* Copy_Input_DataSet = (int*)(malloc(INPUT_DATA_SIZE * sizeof(int)));
    int swap_position;
    int copy_data;
    int low_index_limit = 0;
    int upp_index_limit = INPUT_DATA_SIZE - 1;
    int i = 0, j = 0, n = 0;
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



    printf("\n\n\nIntegration test - Bplus-tree index structure\n\n");
    printf("Input data size                  | (%d records)\n", INPUT_DATA_SIZE);
    printf("Node size - capacity range       | min size: (%d records) - max size: (%d records)\n", MINIMUM_NODE_SIZE, MAXIMUM_NODE_SIZE);
    printf("Node size increment factor       | (%d)\n", NODE_SIZE_INCREMENT_FACTOR);
    printf("Test processes per node size     | (%d)\n\n\n\n", PROCESS_REPETITIONS_PER_NODE_SIZE);



    for (i = 0; i < INPUT_DATA_SIZE; i++)
    {

        Input_DataSet[i] = i;
        Copy_Input_DataSet[i] = i;

    }


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

                itoa(Input_DataSet[i], Primary_Key, 10);

                Retrieved_Data = NULL;
                Retrieved_Data = InitializeBplusTreeData_TableD(Primary_Key, Primary_Key_Size, 0);


                if (BplusTreeInsertData(&Bplustree_Index, Retrieved_Data, &Get_PrimaryKey_TableD, &Deallocate_BplusTreeRecordData_TableD))
                {

                    printf("Test failed\n");
                    printf("Exception (1)\n\n");

                    return 1;

                }


                for(n=0; n < Primary_Key_Size; n++)
                {

                    Primary_Key[n] = ' ';

                }


                for (j = 0; j < i + 1; j++)
                {

                    itoa(Input_DataSet[j], Primary_Key, 10);

                    Retrieved_Data = NULL;
                    Retrieved_Data = InitializeBplusTreeData_TableD(Primary_Key, Primary_Key_Size, 0);


                    if (!BplusTreeInsertData(&Bplustree_Index, Retrieved_Data, &Get_PrimaryKey_TableD, &Deallocate_BplusTreeRecordData_TableD))
                    {

                        printf("Test failed\n");
                        printf("Exception (2)\n\n");

                        return 1;

                    }


                    for(n=0; n < Primary_Key_Size; n++)
                    {

                        Primary_Key[n] = ' ';

                    }

                }


                BplusTreeCheckNodeCapacity(Bplustree_Index, &error_flag);

                if (error_flag)
                {

                    printf("Test failed\n");
                    printf("Exception (3)\n\n");

                    return 1;

                }


                BplusTreeCheckLeafLinkNodes(Bplustree_Index, &improperly_structured_nodes);

                if (improperly_structured_nodes > 0)
                {

                    printf("Test failed\n");
                    printf("Exception (4)\n\n");

                    return 1;

                }


                BplusTreeCheckBalance(Bplustree_Index, Bplustree_Index.height, &unbalanced_nodes);

                if (unbalanced_nodes > 0)
                {

                    printf("Test failed\n");
                    printf("Exception (5)\n\n");

                    return 1;

                }


                BplusTreeCheckInternalNode_ListLinks(Bplustree_Index, &improperly_structured_nodes);

                if (improperly_structured_nodes > 0)
                {

                    printf("Test failed\n");
                    printf("Exception (6)\n\n");

                    return 1;

                }


                if (Bplustree_Index.size != i + 1)
                {

                    printf("Test failed\n");
                    printf("Exception (7)\n\n");

                    return 1;

                }


                BplusTreeComputeHeight(Bplustree_Index, &Btree_Index_height);

                if (Bplustree_Index.height != Btree_Index_height)
                {

                    printf("Test failed\n");
                    printf("Exception (8)\n\n");

                    return 1;

                }




                ////////////////////////////////////////////////// Selection By Primary Key ///////////////////////////////////////////////////




                for (j = 0; j < i + 1; j++)
                {


                    itoa(Input_DataSet[j], Primary_Key, 10);

                    Retrieved_Data = NULL;
                    Retrieved_Data = BplusTreeFastSearchData_ByPrimaryKey(Bplustree_Index, Primary_Key, &Get_PrimaryKey_TableD);


                    if (Retrieved_Data == NULL || strcmp(Get_PrimaryKey_TableD(Retrieved_Data), Primary_Key) != 0 )
                    {

                        printf("Test failed\n");
                        printf("Exception (9)\n\n");

                        return 1;

                    }


                    for(n=0; n < Primary_Key_Size; n++)
                    {

                        Primary_Key[n] = ' ';

                    }

                }


                if (BplusTreeSelectRecordData_ASC(Bplustree_Index, &Selected_Data_List,0, &Get_FieldData_TableD))
                {

                    printf("Test failed\n");
                    printf("Exception (10)\n\n");

                    return 1;

                }


                if (Selected_Data_List.List_Size != i + 1)
                {

                    printf("Test failed\n");
                    printf("Exception (11)\n\n");

                    return 1;

                }


                BplusTreeComputeNodes(Bplustree_Index, &Internal_nodes_number, &Internal_nodes_elements, &Leaf_nodes_number, &Leaf_nodes_elements);

                Leaf_elements = Leaf_nodes_elements - Internal_nodes_elements;


                if (Leaf_elements + Internal_nodes_elements != i + 1)
                {

                    printf("Test failed\n");
                    printf("Exception (12)\n\n");

                    return 1;

                }


                if (Selected_Data_List.List_Size != Leaf_elements + Internal_nodes_elements)
                {

                    printf("Test failed\n");
                    printf("Exception (13)\n\n");

                    return 1;

                }


                if (Internal_nodes_number >= Leaf_nodes_number)
                {

                    printf("Test failed\n");
                    printf("Exception (14)\n\n");

                    return 1;

                }


                if (Internal_nodes_elements >= Leaf_nodes_elements)
                {

                    printf("Test failed\n");
                    printf("Exception (15)\n\n");

                    return 1;

                }


                if (Internal_nodes_elements + 1 != Leaf_nodes_number)
                {

                    printf("Test failed\n");
                    printf("Exception (16)\n\n");

                    return 1;

                }


                internal_elements = 0;
                leaf_only_elements = 0;


                for (j = 0; j < i + 1; j++)
                {


                    itoa(Input_DataSet[j], Primary_Key, 10);

                    Retrieved_Data = NULL;
                    Retrieved_Data = BplusTreeSearch_NodeType_ByRecord(Bplustree_Index, Primary_Key, &exist_flag, &Get_PrimaryKey_TableD);


                    if (Retrieved_Data == NULL || strcmp(Get_PrimaryKey_TableD(Retrieved_Data), Primary_Key) != 0 )
                    {

                        printf("Test failed\n");
                        printf("Exception (17)\n\n");

                        return 1;

                    }


                    for(n=0; n < Primary_Key_Size; n++)
                    {

                        Primary_Key[n] = ' ';

                    }


                    if (exist_flag == 0 || exist_flag > 2)
                    {

                        printf("Test failed\n");
                        printf("Exception (18)\n\n");

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


                if (leaf_only_elements != Leaf_elements)
                {

                    printf("Test failed\n");
                    printf("Exception (19)\n\n");

                    return 1;

                }


                if (internal_elements != Internal_nodes_elements)
                {

                    printf("Test failed\n");
                    printf("Exception (20)\n\n");

                    return 1;

                }


                if (Leaf_nodes_elements - internal_elements != Leaf_elements)
                {

                    printf("Test failed\n");
                    printf("Exception (21)\n\n");

                    return 1;

                }


                DeleteBplusList(&Selected_Data_List);

            }




            ////////////////////////////////////////////////// Full Selection ///////////////////////////////////////////////////




            if (BplusTreeSelectRecordData_ASC(Bplustree_Index, &Selected_Data_List,0, &Get_FieldData_TableD))
            {

                printf("Test failed\n");
                printf("Exception (22)\n\n");

                return 1;

            }


            if (Selected_Data_List.List_Size != INPUT_DATA_SIZE)
            {

                printf("Test failed\n");
                printf("Exception (23)\n\n");

                return 1;

            }


            CurrentNode = Selected_Data_List.List_HeadNode->List_NextNode;
            index = 0;


            while (CurrentNode != NULL)
            {

                if ( strcmp(Get_PrimaryKey_TableD(CurrentNode->RecordData), Get_PrimaryKey_TableD(CurrentNode->List_PrevNode->RecordData)) <= 0 )
                {

                    printf("Test failed\n");
                    printf("Exception (24)\n\n");

                    return 1;

                }



                CurrentNode = CurrentNode->List_NextNode;
                index++;

            }


            DeleteBplusList(&Selected_Data_List);


            if (BplusTreeSelectRecordData_DESC(Bplustree_Index, &Selected_Data_List,0, &Get_FieldData_TableD))
            {

                printf("Test failed\n");
                printf("Exception (25)\n\n");

                return 1;

            }


            if (Selected_Data_List.List_Size != INPUT_DATA_SIZE)
            {

                printf("Test failed\n");
                printf("Exception (26)\n\n");

                return 1;

            }


            CurrentNode = Selected_Data_List.List_HeadNode->List_NextNode;
            index = INPUT_DATA_SIZE - 1;


            while (CurrentNode != NULL)
            {


                if ( strcmp(Get_PrimaryKey_TableD(CurrentNode->RecordData), Get_PrimaryKey_TableD(CurrentNode->List_PrevNode->RecordData)) >= 0 )
                {

                    printf("Test failed\n");
                    printf("Exception (27)\n\n");

                    return 1;

                }


                CurrentNode = CurrentNode->List_NextNode;
                index--;

            }


            DeleteBplusList(&Selected_Data_List);


            empty_flag = BplusTreeSelectRecordData_Rec_ASC(Bplustree_Index, &Selected_Data_List);

            BplusTreeComputeNodes(Bplustree_Index, &Internal_nodes_number, &Internal_nodes_elements, &Leaf_nodes_number, &Leaf_nodes_elements);


            if (Selected_Data_List.List_Size != Internal_nodes_elements + Leaf_nodes_elements)
            {

                printf("Test failed\n");
                printf("Exception (28)\n\n");

                return 1;

            }


            duplicate_pk_values = 0;

            if (!empty_flag)
            {

                CurrentNode = Selected_Data_List.List_HeadNode->List_NextNode;

                while (CurrentNode != NULL)
                {

                    if ( strcmp(Get_PrimaryKey_TableD(CurrentNode->RecordData), Get_PrimaryKey_TableD(CurrentNode->List_PrevNode->RecordData)) < 0 )
                    {

                        printf("Test failed\n");
                        printf("Exception (29)\n\n");

                        return 1;

                    }
                    else {

                        if ( strcmp(Get_PrimaryKey_TableD(CurrentNode->RecordData), Get_PrimaryKey_TableD(CurrentNode->List_PrevNode->RecordData)) == 0 )
                        {

                            duplicate_pk_values++;

                        }

                    }


                    CurrentNode = CurrentNode->List_NextNode;

                }


            }
            else {

                printf("Test failed\n");
                printf("Exception (30)\n\n");

                return 1;

            }


            if (duplicate_pk_values != Internal_nodes_elements)
            {

                printf("Test failed\n");
                printf("Exception (31)\n\n");

                return 1;

            }


            DeleteBplusList(&Selected_Data_List);

            BplusTreeDrop(&Bplustree_Index, &Deallocate_BplusTreeRecordData_TableD);

        }


        printf("(Test passed)\n\n");

    }


    free(Input_DataSet);
    free(Copy_Input_DataSet);
    free(Primary_Key);


    printf("\n");
    printf("(Integration test passed)\n\n");


    return 0;


}

