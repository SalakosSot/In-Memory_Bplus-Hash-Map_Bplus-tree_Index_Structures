
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "BplusTree.h"



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



bool InitializeBplustree(

        BplusTree* BplusTree,
        int node_size

)
{


    if (node_size < 3)
    {

        printf("Initialization is unsuccessful\n\n");

        return 1;

    }


    BplusTree->Root = NULL;
    BplusTree->node_size = node_size;
    BplusTree->size = 0;
    BplusTree->height = 0;

    return 0;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool BplusTreeInsertData(

        BplusTree* BplusTree,
        BplusTreeData RecordData,

        BplusTreePrimaryKey (*Get_Record_PrimaryKey)(BplusTreeData),
        bool (*Deallocate_Record_Data)(BplusTreeData)

)
{


    if (BplusTree->Root == NULL)
    {

        BplusTree->Root = (BplusTreeNode)(malloc(sizeof(struct BplusTreeNode)));
        BplusTree->Root->leaf_type = 1;
        BplusTree->Root->NextLeafNode = NULL;
        BplusTree->Root->PreviousLeafNode = NULL;
        BplusTree->Root->array_stored_elements = 1;

        BplusTree->Root->RecordDataArray = (BplusTreeData*)(malloc((BplusTree->node_size / 2) * sizeof(BplusTreeData)));
        BplusTree->Root->RecordDataArray[0] = RecordData;

        BplusTree->Root->NodeLinkArray = NULL;
        BplusTree->Root->array_available_capacity = BplusTree->node_size / 2;


        BplusTree->size += 1;
        BplusTree->height += 1;

        return 0;

    }


    if (BplusTreeInsertNode_RootBreakTool(BplusTree, RecordData, BplusTree->node_size, Get_Record_PrimaryKey, Deallocate_Record_Data))
    {

        return 1;

    }

    return 0;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool BplusTreeInsertNode_RootBreakTool(

        BplusTree* BplusTree,
        BplusTreeData RecordData,
        int node_size,

        BplusTreePrimaryKey (*Get_Record_PrimaryKey)(BplusTreeData),
        bool (*Deallocate_Record_Data)(BplusTreeData)

)
{


    BplusTreeNode BTreeNodeLeft = NULL;
    BplusTreeNode BTreeNodeRight = NULL;
    BplusTreeData Node_MiddleRecordData;
    bool flag_duplicate_record = 0;


    BTreeNodeRight = BplusTreeInsertNode_Tool( BplusTree->Root,
                                               RecordData,
                                               &Node_MiddleRecordData,
                                               node_size,
                                               &flag_duplicate_record,
                                               Get_Record_PrimaryKey,
                                               Deallocate_Record_Data );


    if (flag_duplicate_record)
    {

        return 1;

    }


    if (BTreeNodeRight != NULL)
    {

        BTreeNodeLeft = (BplusTreeNode)(malloc(sizeof(struct BplusTreeNode)));
        BTreeNodeLeft->RecordDataArray = (BplusTreeData*)(malloc(BplusTree->Root->array_available_capacity * sizeof(BplusTreeData)));


        if (!BplusTree->Root->leaf_type)
        {

            BTreeNodeLeft->NodeLinkArray = (BplusTreeNode*)(malloc((node_size / 2 + 1) * sizeof(BplusTreeNode)));

        }
        else {

            BTreeNodeLeft->NodeLinkArray = NULL;

        }


        BTreeNodeLeft->leaf_type = BplusTree->Root->leaf_type;
        BTreeNodeLeft->array_stored_elements = BplusTree->Root->array_stored_elements;
        BTreeNodeLeft->array_available_capacity = BplusTree->Root->array_available_capacity;


        if (BplusTree->Root->leaf_type)
        {

            BplusTree->Root->NextLeafNode = NULL;
            BplusTree->Root->PreviousLeafNode = NULL;

            BTreeNodeLeft->PreviousLeafNode = NULL;
            BTreeNodeRight->NextLeafNode = NULL;

            BTreeNodeLeft->NextLeafNode = BTreeNodeRight;
            BTreeNodeRight->PreviousLeafNode = BTreeNodeLeft;

        }
        else {

            BTreeNodeLeft->NextLeafNode = NULL;
            BTreeNodeLeft->PreviousLeafNode = NULL;

        }


        memmove( BTreeNodeLeft->RecordDataArray,
                 BplusTree->Root->RecordDataArray,
                 sizeof(*(BplusTree->Root->RecordDataArray)) * BTreeNodeLeft->array_stored_elements );


        if (!BplusTree->Root->leaf_type)
        {

            memmove( BTreeNodeLeft->NodeLinkArray,
                     BplusTree->Root->NodeLinkArray,
                     sizeof(*(BplusTree->Root->NodeLinkArray)) * (BTreeNodeLeft->array_stored_elements + 1) );

        }


        BplusTree->Root->RecordDataArray = (BplusTreeData*)(realloc(BplusTree->Root->RecordDataArray, (node_size / 2) * sizeof(BplusTreeData)));


        if (!BplusTree->Root->leaf_type)
        {

            BplusTree->Root->NodeLinkArray = (BplusTreeNode*)(realloc(BplusTree->Root->NodeLinkArray, (node_size / 2 + 1) * sizeof(BplusTreeNode)));

        }
        else {

            BplusTree->Root->NodeLinkArray = (BplusTreeNode*)(malloc((node_size / 2 + 1) * sizeof(BplusTreeNode)));

        }


        BplusTree->Root->array_stored_elements = 1;
        BplusTree->Root->leaf_type = 0;

        BplusTree->Root->RecordDataArray[0] = Node_MiddleRecordData;
        BplusTree->Root->NodeLinkArray[0] = BTreeNodeLeft;
        BplusTree->Root->NodeLinkArray[1] = BTreeNodeRight;
        BplusTree->Root->array_available_capacity = node_size / 2;


        BplusTree->height += 1;

    }


    BplusTree->size += 1;

    return 0;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BplusTreeNode BplusTreeInsertNode_Tool(

        BplusTreeNode CurrentNode,
        BplusTreeData RecordData,
        BplusTreeData* Node_MiddleRecordData,
        int node_size,
        bool* flag_duplicate_record,

        BplusTreePrimaryKey (*Get_Record_PrimaryKey)(BplusTreeData),
        bool (*Deallocate_Record_Data)(BplusTreeData)

)
{


    BplusTreeNode BTreeNodeRight = NULL;
    int record_position;
    BplusTreeData middle_record_data;


    record_position = SearchBplusTreeNode_Record_ByPrimaryKey( CurrentNode->array_stored_elements,
                                                               CurrentNode->RecordDataArray,
                                                               Get_Record_PrimaryKey(RecordData),
                                                               Get_Record_PrimaryKey );


    if ( (record_position < CurrentNode->array_stored_elements)
         && (Get_Record_PrimaryKey(CurrentNode->RecordDataArray[record_position]) == Get_Record_PrimaryKey(RecordData)) )
    {

        //printf("Duplicate index : %d\n\n", CurrentNode->RecordDataArray[record_position]->Record_PrimaryKey);

        if (RecordData != CurrentNode->RecordDataArray[record_position])
        {

            Deallocate_Record_Data(RecordData);

        }


        *flag_duplicate_record = 1;


        return NULL;

    }


    if (CurrentNode->leaf_type)
    {

        if (CurrentNode->array_available_capacity == (node_size / 2) && CurrentNode->array_stored_elements == (node_size / 2))
        {

            CurrentNode->RecordDataArray = (BplusTreeData*)(realloc(CurrentNode->RecordDataArray, node_size * sizeof(BplusTreeData)));
            CurrentNode->array_available_capacity = node_size;

        }


        memmove(&CurrentNode->RecordDataArray[record_position + 1],
                &CurrentNode->RecordDataArray[record_position],
                sizeof(*(CurrentNode->RecordDataArray)) * (CurrentNode->array_stored_elements - record_position) );

        CurrentNode->RecordDataArray[record_position] = RecordData;
        CurrentNode->array_stored_elements++;


        if (CurrentNode->array_stored_elements == node_size)
        {

            int middle_primary_key;


            if ((CurrentNode->array_stored_elements % 2) == 0)
            {

                middle_primary_key = (CurrentNode->array_stored_elements / 2) - 1;

            }
            else {

                middle_primary_key = CurrentNode->array_stored_elements / 2;

            }


            *Node_MiddleRecordData = CurrentNode->RecordDataArray[middle_primary_key];

            BTreeNodeRight = (BplusTreeNode)(malloc(sizeof(struct BplusTreeNode)));

            BTreeNodeRight->RecordDataArray = (BplusTreeData*)(malloc((node_size / 2) * sizeof(BplusTreeData)));
            BTreeNodeRight->NodeLinkArray = NULL;

            BTreeNodeRight->array_stored_elements = CurrentNode->array_stored_elements - middle_primary_key - 1;
            BTreeNodeRight->leaf_type = 1;
            BTreeNodeRight->array_available_capacity = node_size / 2;


            if (CurrentNode->NextLeafNode == NULL)
            {

                CurrentNode->NextLeafNode = BTreeNodeRight;
                BTreeNodeRight->PreviousLeafNode = CurrentNode;

                BTreeNodeRight->NextLeafNode = NULL;

            }
            else {

                BTreeNodeRight->NextLeafNode = CurrentNode->NextLeafNode;
                CurrentNode->NextLeafNode->PreviousLeafNode = BTreeNodeRight;

                CurrentNode->NextLeafNode = BTreeNodeRight;
                BTreeNodeRight->PreviousLeafNode = CurrentNode;

            }


            memmove( BTreeNodeRight->RecordDataArray,
                     &CurrentNode->RecordDataArray[middle_primary_key + 1],
                     sizeof(*(CurrentNode->RecordDataArray)) * BTreeNodeRight->array_stored_elements );


            if ((CurrentNode->array_stored_elements % 2) == 0)
            {

                CurrentNode->RecordDataArray = (BplusTreeData*)(realloc(CurrentNode->RecordDataArray, (node_size / 2) * sizeof(BplusTreeData)));

                CurrentNode->array_available_capacity = node_size / 2;

            }


            CurrentNode->array_stored_elements = middle_primary_key + 1;


            return BTreeNodeRight;

        }


        return NULL;

    }


    BTreeNodeRight = BplusTreeInsertNode_Tool( CurrentNode->NodeLinkArray[record_position],
                                               RecordData,
                                               &middle_record_data,
                                               node_size,
                                               flag_duplicate_record,
                                               Get_Record_PrimaryKey,
                                               Deallocate_Record_Data );


    if (BTreeNodeRight != NULL)
    {

        if (CurrentNode->array_available_capacity == (node_size / 2) && CurrentNode->array_stored_elements == (node_size / 2))
        {

            CurrentNode->RecordDataArray = (BplusTreeData*)(realloc(CurrentNode->RecordDataArray, node_size * sizeof(BplusTreeData)));
            CurrentNode->NodeLinkArray = (BplusTreeNode*)(realloc(CurrentNode->NodeLinkArray, (node_size + 1) * sizeof(BplusTreeNode)));
            CurrentNode->array_available_capacity = node_size;

        }


        memmove( &CurrentNode->RecordDataArray[record_position + 1],
                 &CurrentNode->RecordDataArray[record_position],
                 sizeof(*(CurrentNode->RecordDataArray)) * (CurrentNode->array_stored_elements - record_position) );

        memmove( &CurrentNode->NodeLinkArray[record_position + 2],
                 &CurrentNode->NodeLinkArray[record_position + 1],
                 sizeof(*(CurrentNode->NodeLinkArray)) * (CurrentNode->array_stored_elements - record_position) );

        CurrentNode->RecordDataArray[record_position] = middle_record_data;
        CurrentNode->NodeLinkArray[record_position + 1] = BTreeNodeRight;
        CurrentNode->array_stored_elements++;

    }
    else {

        return NULL;

    }


    if (CurrentNode->array_stored_elements == node_size)
    {

        int middle_primary_key;


        if ((CurrentNode->array_stored_elements % 2) == 0)
        {

            middle_primary_key = (CurrentNode->array_stored_elements / 2) - 1;

        }
        else {

            middle_primary_key = CurrentNode->array_stored_elements / 2;

        }


        *Node_MiddleRecordData = CurrentNode->RecordDataArray[middle_primary_key];

        BTreeNodeRight = (BplusTreeNode)(malloc(sizeof(struct BplusTreeNode)));

        BTreeNodeRight->RecordDataArray = (BplusTreeData*)(malloc((node_size / 2) * sizeof(BplusTreeData)));
        BTreeNodeRight->NodeLinkArray = (BplusTreeNode*)(malloc((node_size / 2 + 1) * sizeof(BplusTreeNode)));

        BTreeNodeRight->array_stored_elements = CurrentNode->array_stored_elements - middle_primary_key - 1;
        BTreeNodeRight->leaf_type = 0;
        BTreeNodeRight->NextLeafNode = NULL;
        BTreeNodeRight->PreviousLeafNode = NULL;
        BTreeNodeRight->array_available_capacity = node_size / 2;

        memmove( BTreeNodeRight->RecordDataArray,
                 &CurrentNode->RecordDataArray[middle_primary_key + 1],
                 sizeof(*(CurrentNode->RecordDataArray)) * BTreeNodeRight->array_stored_elements );

        memmove( BTreeNodeRight->NodeLinkArray,
                 &CurrentNode->NodeLinkArray[middle_primary_key + 1],
                 sizeof(*(CurrentNode->NodeLinkArray)) * (BTreeNodeRight->array_stored_elements + 1) );

        CurrentNode->RecordDataArray = (BplusTreeData*)(realloc(CurrentNode->RecordDataArray, (node_size / 2) * sizeof(BplusTreeData)));
        CurrentNode->NodeLinkArray = (BplusTreeNode*)(realloc(CurrentNode->NodeLinkArray, (node_size / 2 + 1) * sizeof(BplusTreeNode)));

        CurrentNode->array_stored_elements = middle_primary_key;
        CurrentNode->array_available_capacity = node_size / 2;


        return BTreeNodeRight;

    }


    return NULL;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int SearchBplusTreeNode_Record_ByPrimaryKey(

        int array_stored_elements,
        BplusTreeData* RecordDataArray,
        BplusTreePrimaryKey PrimaryKey,

        BplusTreePrimaryKey (*Get_Record_PrimaryKey)(BplusTreeData)

)
{


    return BplusTree_Interpolation_Search(array_stored_elements, RecordDataArray, PrimaryKey, Get_Record_PrimaryKey);


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int BplusTree_Binary_Search(

        int array_stored_elements,
        BplusTreeData* RecordDataArray,
        BplusTreePrimaryKey PrimaryKey,

        BplusTreePrimaryKey (*Get_Record_PrimaryKey)(BplusTreeData)

)
{


    int first_index = 0;
    int last_index = array_stored_elements - 1;
    int middle_index = (first_index + last_index) / 2;
    int record_position = 0;


    if (PrimaryKey > Get_Record_PrimaryKey(RecordDataArray[array_stored_elements - 1]))
    {

        return array_stored_elements;

    }


    if (PrimaryKey < Get_Record_PrimaryKey(RecordDataArray[0]))
    {

        return 0;

    }


    while (first_index <= last_index)
    {

        if (Get_Record_PrimaryKey(RecordDataArray[middle_index]) < PrimaryKey)
        {

            record_position = middle_index + 1;
            first_index = middle_index + 1;

        }
        else if (Get_Record_PrimaryKey(RecordDataArray[middle_index]) == PrimaryKey)
        {

            record_position = middle_index;
            break;

        }
        else {

            record_position = middle_index;
            last_index = middle_index - 1;

        }


        middle_index = (first_index + last_index) / 2;


    }


    return record_position;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int BplusTree_Interpolation_Search(

        int array_stored_elements,
        BplusTreeData* RecordDataArray,
        BplusTreePrimaryKey PrimaryKey,

        BplusTreePrimaryKey (*Get_Record_PrimaryKey)(BplusTreeData)

)
{


    int low_index = 0;
    int high_index = (array_stored_elements - 1);
    BplusTreePrimaryKey primary_key_low_index = 0;
    BplusTreePrimaryKey primary_key_high_index = 0;
    int record_position = 0;
    int range_position = 0;


    if (PrimaryKey > Get_Record_PrimaryKey(RecordDataArray[high_index]))
    {

        return high_index + 1;

    }


    if (PrimaryKey < Get_Record_PrimaryKey(RecordDataArray[low_index]))
    {

        return low_index;

    }


    while ( low_index <= high_index && PrimaryKey >= Get_Record_PrimaryKey(RecordDataArray[low_index])
            && PrimaryKey <= Get_Record_PrimaryKey(RecordDataArray[high_index]) )
    {

        if (low_index == high_index)
        {

            if (Get_Record_PrimaryKey(RecordDataArray[low_index]) == PrimaryKey)
            {

                return low_index;

            }


            if (PrimaryKey > Get_Record_PrimaryKey(RecordDataArray[low_index]))
            {

                return low_index + 1;

            }


            return low_index;

        }


        primary_key_low_index = Get_Record_PrimaryKey(RecordDataArray[low_index]);
        primary_key_high_index = Get_Record_PrimaryKey(RecordDataArray[high_index]);

        range_position = low_index + (((double)(high_index - low_index)
                         / (primary_key_high_index - primary_key_low_index)) * (PrimaryKey - primary_key_low_index));



        if (Get_Record_PrimaryKey(RecordDataArray[range_position]) == PrimaryKey)
        {

            return range_position;

        }
        else if (Get_Record_PrimaryKey(RecordDataArray[range_position]) < PrimaryKey)
        {

            record_position = range_position + 1;
            low_index = range_position + 1;

        }
        else
        {

            record_position = range_position;
            high_index = range_position - 1;

        }

    }


    return record_position;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BplusTreeDisplayInOrder(

        BplusTree BplusTree,

        BplusTreePrimaryKey (*Get_Record_PrimaryKey)(BplusTreeData)

)
{


    if (BplusTree.Root == NULL)
    {

        printf("Bplus-tree is empty\n\n");

        return;

    }


    BplusTreeInOrderTraversal(BplusTree.Root, Get_Record_PrimaryKey);

    return;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BplusTreeInOrderTraversal(

        BplusTreeNode CurrentNode,

        BplusTreePrimaryKey (*Get_Record_PrimaryKey)(BplusTreeData)

)
{


    if (CurrentNode->leaf_type)
    {


        for (int i = 0; i < CurrentNode->array_stored_elements; i++)
        {

            printf(" (Leaf Node): (%d)", Get_Record_PrimaryKey(CurrentNode->RecordDataArray[i]));

        }
        printf("\n");

        return;

    }


    for (int i = 0; i < CurrentNode->array_stored_elements + 1; i++)
    {

        BplusTreeInOrderTraversal(CurrentNode->NodeLinkArray[i], Get_Record_PrimaryKey);


        if ((i + 1) < (CurrentNode->array_stored_elements + 1))
        {

            printf(" (Internal Node): (%d)\\n", Get_Record_PrimaryKey(CurrentNode->RecordDataArray[i]));

        }

    }


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BplusTreeData BplusTreeSearchData(

        BplusTree BplusTree,
        BplusTreePrimaryKey PrimaryKey,

        BplusTreePrimaryKey (*Get_Record_PrimaryKey)(BplusTreeData)

)
{


    if (BplusTree.Root == NULL)
    {

        printf("Bplus-tree is empty\n\n");

        return NULL;

    }


    return BplusTreeSearch_Tool(BplusTree.Root, PrimaryKey, Get_Record_PrimaryKey);


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BplusTreeData BplusTreeSearch_Tool(

        BplusTreeNode CurrentNode,
        BplusTreePrimaryKey PrimaryKey,

        BplusTreePrimaryKey (*Get_Record_PrimaryKey)(BplusTreeData)

)
{


    int record_position = CurrentNode->array_stored_elements;


    for (int i = 0; i < CurrentNode->array_stored_elements; i++)
    {

        printf("Search Operation - Primary Key Field: (%d)\n", Get_Record_PrimaryKey(CurrentNode->RecordDataArray[i]));


        if (PrimaryKey <= Get_Record_PrimaryKey(CurrentNode->RecordDataArray[i]))
        {

            if (PrimaryKey == Get_Record_PrimaryKey(CurrentNode->RecordDataArray[i]) && CurrentNode->leaf_type)
            {

                return CurrentNode->RecordDataArray[i];

            }


            record_position = i;

            break;

        }

    }


    printf("--------------------- Next Level ---------------------\n\n");


    if (!CurrentNode->leaf_type)
    {

        return BplusTreeSearch_Tool(CurrentNode->NodeLinkArray[record_position], PrimaryKey, Get_Record_PrimaryKey);

    }


    return NULL;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BplusTreeData BplusTreeFastSearchData_ByPrimaryKey(

        BplusTree BplusTree,
        BplusTreePrimaryKey PrimaryKey,

        BplusTreePrimaryKey (*Get_Record_PrimaryKey)(BplusTreeData)

)
{


    if (BplusTree.Root == NULL)
    {

        //printf("Bplus-tree is empty\n\n");

        return NULL;

    }


    return BplusTreeFastSearch_Tool(BplusTree.Root, PrimaryKey, Get_Record_PrimaryKey);


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BplusTreeData BplusTreeFastSearch_Tool(

        BplusTreeNode CurrentNode,
        BplusTreePrimaryKey PrimaryKey,

        BplusTreePrimaryKey (*Get_Record_PrimaryKey)(BplusTreeData)

)
{


    int record_position = CurrentNode->array_stored_elements;

    record_position = SearchBplusTreeNode_Record_ByPrimaryKey( CurrentNode->array_stored_elements,
                                                               CurrentNode->RecordDataArray,
                                                               PrimaryKey,
                                                               Get_Record_PrimaryKey );


    if ( (record_position < CurrentNode->array_stored_elements)
         && (Get_Record_PrimaryKey(CurrentNode->RecordDataArray[record_position]) == PrimaryKey) )
    {

        return CurrentNode->RecordDataArray[record_position];

    }


    if (!CurrentNode->leaf_type)
    {

        return BplusTreeFastSearch_Tool(CurrentNode->NodeLinkArray[record_position], PrimaryKey, Get_Record_PrimaryKey);

    }


    return NULL;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BplusTreeData InitializeBplusTreeData_TableC(

        BplusTreePrimaryKey Record_PrimaryKey,
        int Field_Data

)
{


    BplusTreeData NewRecordData = (BplusTreeData)(malloc(sizeof(struct BplusTreeRecordData)));

    NewRecordData->RecordData = (TableC_RecordData)(malloc(sizeof(struct TableC_RecordData)));
    NewRecordData->table_id = 3;

    ((TableC_RecordData)(NewRecordData->RecordData))->Record_PrimaryKey = Record_PrimaryKey;
    ((TableC_RecordData)(NewRecordData->RecordData))->Field_Data = Field_Data;

    return NewRecordData;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BplusTreePrimaryKey Get_PrimaryKey_TableC(

        BplusTreeData Input_RecordData

)
{


    return ((TableC_RecordData)(Input_RecordData->RecordData))->Record_PrimaryKey;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int Get_FieldData_TableC(

        BplusTreeData Input_RecordData

)
{


    return ((TableC_RecordData)(Input_RecordData->RecordData))->Field_Data;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool Deallocate_BplusTreeRecordData_TableC(

        BplusTreeData RecordData_ToBe_Deleted

)
{


    if (RecordData_ToBe_Deleted == NULL)
    {

        printf("Record data - not found\n\n");

        return 1;

    }


    free(((TableC_RecordData)(RecordData_ToBe_Deleted->RecordData)));
    free(RecordData_ToBe_Deleted);

    return 0;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool BplusTreeDrop(

        BplusTree* BplusTree,

        bool (*Deallocate_Record_Data)(BplusTreeData)

)
{


    if (BplusTree->Root == NULL)
    {

        printf("Bplus-tree is empty\n\n");

        return 1;

    }


    BplusTreeDropNode(BplusTree->Root, Deallocate_Record_Data);

    InitializeBplustree(BplusTree, BplusTree->node_size);

    return 0;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BplusTreeDropNode(

        BplusTreeNode CurrentNode,

        bool (*Deallocate_Record_Data)(BplusTreeData)

)
{


    if (CurrentNode->leaf_type)
    {

        for (int i = 0; i < CurrentNode->array_stored_elements; i++)
        {

            Deallocate_Record_Data(CurrentNode->RecordDataArray[i]);

        }


        free(CurrentNode->RecordDataArray);
        free(CurrentNode);

        return;

    }


    for (int i = 0; i < CurrentNode->array_stored_elements; i++)
    {

        BplusTreeDropNode(CurrentNode->NodeLinkArray[i], Deallocate_Record_Data);

    }


    BplusTreeDropNode(CurrentNode->NodeLinkArray[CurrentNode->array_stored_elements], Deallocate_Record_Data);


    free(CurrentNode->RecordDataArray);
    free(CurrentNode->NodeLinkArray);
    free(CurrentNode);


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool BplusTreeSelectRecordData_ASC(

        BplusTree BplusTree,
        struct DoubleLinkedBplusList* SelectionResultList,
        int Field_Data,

        int (*Get_Record_FieldData)(BplusTreeData)

)
{


    if (BplusTree.Root == NULL)
    {

        printf("Bplus-tree is Empty\n\n");

        return 1;

    }


    BplusTreeNode CurrentNode = BplusTreeLocateLeftLeafNode(BplusTree);


    while (CurrentNode != NULL)
    {

        for (int i = 0; i < CurrentNode->array_stored_elements; i++)
        {

            if (Get_Record_FieldData(CurrentNode->RecordDataArray[i]) == Field_Data)
            {

                InsertBplusListNode_Last(SelectionResultList, CurrentNode->RecordDataArray[i]);

            }

        }


        CurrentNode = CurrentNode->NextLeafNode;

    }


    if (SelectionResultList->List_HeadNode == NULL)
    {

        printf("List is Empty\n\n");

        return 1;

    }


    return 0;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BplusTreeNode BplusTreeLocateLeftLeafNode(

        BplusTree BplusTree

)
{


    if (BplusTree.Root == NULL)
    {

        printf("Bplus-tree is Empty\n\n");

        return NULL;

    }


    while (!BplusTree.Root->leaf_type)
    {

        BplusTree.Root = BplusTree.Root->NodeLinkArray[0];

    }


    return BplusTree.Root;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool BplusTreeSelectRecordData_DESC(

        BplusTree BplusTree,
        struct DoubleLinkedBplusList* SelectionResultList,
        int Field_Data,

        int (*Get_Record_FieldData)(BplusTreeData)

)
{


    if (BplusTree.Root == NULL)
    {

        printf("Bplus-tree is Empty\n\n");

        return 1;

    }


    BplusTreeNode CurrentNode = BplusTreeLocateRightLeafNode(BplusTree);


    while (CurrentNode != NULL)
    {

        for (int i = CurrentNode->array_stored_elements - 1; i >= 0; i--)
        {

            if (Get_Record_FieldData(CurrentNode->RecordDataArray[i]) == Field_Data)
            {

                InsertBplusListNode_Last(SelectionResultList, CurrentNode->RecordDataArray[i]);

            }

        }


        CurrentNode = CurrentNode->PreviousLeafNode;

    }


    if (SelectionResultList->List_HeadNode == NULL)
    {

        printf("List is Empty\n\n");

        return 1;

    }


    return 0;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BplusTreeNode BplusTreeLocateRightLeafNode(

        BplusTree BplusTree

)
{


    if (BplusTree.Root == NULL)
    {

        printf("Bplus-tree is Empty\n\n");

        return NULL;

    }


    while (!BplusTree.Root->leaf_type)
    {

        BplusTree.Root = BplusTree.Root->NodeLinkArray[BplusTree.Root->array_stored_elements];

    }


    return BplusTree.Root;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool BplusTreeSelectRecordData_Rec_ASC(

        BplusTree BplusTree,
        struct DoubleLinkedBplusList* SelectionResultList

)
{


    if (BplusTree.Root == NULL)
    {

        printf("Bplus-tree is empty\n\n");

        return 1;

    }


    BplusTreeSelectRecordData_Rec_ASC_Tool(BplusTree.Root, SelectionResultList);


    if (SelectionResultList->List_HeadNode == NULL)
    {

        return 1;

    }


    return 0;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BplusTreeSelectRecordData_Rec_ASC_Tool(

        BplusTreeNode CurrentNode,
        struct DoubleLinkedBplusList* SelectionResultList

)
{


    if (CurrentNode->leaf_type)
    {

        for (int i = 0; i < CurrentNode->array_stored_elements; i++)
        {

            InsertBplusListNode_Last(SelectionResultList, CurrentNode->RecordDataArray[i]);

        }


        return;

    }


    for (int i = 0; i < CurrentNode->array_stored_elements; i++)
    {

        BplusTreeSelectRecordData_Rec_ASC_Tool(CurrentNode->NodeLinkArray[i], SelectionResultList);

        InsertBplusListNode_Last(SelectionResultList, CurrentNode->RecordDataArray[i]);

    }


    BplusTreeSelectRecordData_Rec_ASC_Tool(CurrentNode->NodeLinkArray[CurrentNode->array_stored_elements], SelectionResultList);


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool BplusTreeComputeNodes(

        BplusTree BplusTree,
        int* internal_nodes,
        int* internal_nodes_records,
        int* leaf_nodes,
        int* leaf_nodes_records

)
{


    if (BplusTree.Root == NULL)
    {

        printf("BplusTree is empty\n\n");

        return 1;

    }


    *internal_nodes = 0;
    *internal_nodes_records = 0;
    *leaf_nodes = 0;
    *leaf_nodes_records = 0;

    BplusTreeComputeNodes_Tool(BplusTree.Root, internal_nodes, internal_nodes_records, leaf_nodes, leaf_nodes_records);

    return 0;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BplusTreeComputeNodes_Tool(

        BplusTreeNode CurrentNode,
        int* internal_nodes,
        int* internal_nodes_records,
        int* leaf_nodes,
        int* leaf_nodes_records

)
{


    if (CurrentNode->leaf_type)
    {

        *leaf_nodes += 1;
        *leaf_nodes_records += CurrentNode->array_stored_elements;

        return;

    }


    *internal_nodes += 1;
    *internal_nodes_records += CurrentNode->array_stored_elements;


    for (int i = 0; i < CurrentNode->array_stored_elements + 1; i++)
    {

        BplusTreeComputeNodes_Tool(CurrentNode->NodeLinkArray[i], internal_nodes, internal_nodes_records, leaf_nodes, leaf_nodes_records);

    }


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool BplusTreeComputeHeight(

        BplusTree BplusTree,
        int* height

)
{


    if (BplusTree.Root == NULL)
    {

        printf("Unable to compute height\n\n");

        return 1;

    }


    *height = 0;


    while (!BplusTree.Root->leaf_type)
    {

        BplusTree.Root = BplusTree.Root->NodeLinkArray[0];

        *height += 1;

    }


    *height += 1;


    return 0;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool BplusTreeCheckNodeCapacity(

        BplusTree BplusTree,
        bool* error_flag

)
{


    if (BplusTree.Root == NULL)
    {

        printf("Bplus-tree is empty\n\n");

        return 1;

    }


    *error_flag = 0;
    BplusTreeCheckNodeCapacity_Tool(BplusTree.Root, BplusTree.node_size, error_flag);


    return 0;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BplusTreeCheckNodeCapacity_Tool(

        BplusTreeNode CurrentNode,
        int node_size,
        bool* error_flag

)
{


    if (CurrentNode->leaf_type)
    {


        if (CurrentNode->array_stored_elements >= node_size)
        {

            printf("Exception - B-tree leaf node number of records addresses\n");

            *error_flag = 1;

        }


        if (!(CurrentNode->array_available_capacity == node_size || CurrentNode->array_available_capacity == node_size / 2))
        {

            printf("Exception - B-tree leaf node capacity\n");

            *error_flag = 1;

        }


        if (CurrentNode->array_available_capacity == node_size / 2 && CurrentNode->array_stored_elements > CurrentNode->array_available_capacity)
        {

            printf("Exception (1) - B-tree leaf node capacity in compare with records addresses\n");

            *error_flag = 1;

        }


        if (CurrentNode->array_available_capacity == node_size && CurrentNode->array_stored_elements < CurrentNode->array_available_capacity / 2)
        {

            printf("Exception (2) - B-tree leaf node capacity in compare with records addresses\n");

            *error_flag = 1;

        }


        return;

    }


    if (CurrentNode->array_stored_elements >= node_size)
    {

        printf("Exception - B-tree internal node number of records addresses\n");

        *error_flag = 1;

    }


    if (!(CurrentNode->array_available_capacity == node_size || CurrentNode->array_available_capacity == node_size / 2))
    {

        printf("Exception - B-tree internal node capacity\n");

        *error_flag = 1;

    }


    if (CurrentNode->array_available_capacity == node_size / 2 && CurrentNode->array_stored_elements > CurrentNode->array_available_capacity)
    {

        printf("Exception (1) - B-tree internal node capacity in compare with records addresses\n");

        *error_flag = 1;

    }


    if (CurrentNode->array_available_capacity == node_size && CurrentNode->array_stored_elements < CurrentNode->array_available_capacity / 2)
    {

        printf("Exception (2) - B-tree internal node capacity in compare with records addresses\n");

        *error_flag = 1;

    }


    for (int i = 0; i < CurrentNode->array_stored_elements + 1; i++)
    {

        BplusTreeCheckNodeCapacity_Tool(CurrentNode->NodeLinkArray[i], node_size, error_flag);

    }


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool BplusTreeCheckBalance(

        BplusTree BplusTree,
        int Bplustree_height,
        int* unbalanced_nodes

)
{


    if (BplusTree.Root == NULL)
    {

        printf("Bplus-tree is empty\n\n");

        return 1;

    }


    int node_height = 1;
    *unbalanced_nodes = 0;
    BplusTreeCheckBalance_Tool(BplusTree.Root, node_height, Bplustree_height, unbalanced_nodes);


    return 0;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BplusTreeCheckBalance_Tool(

        BplusTreeNode CurrentNode,
        int node_height,
        int BplusTree_height,
        int* unbalanced_nodes

)
{


    if (CurrentNode->leaf_type)
    {

        if (node_height != BplusTree_height)
        {

            *unbalanced_nodes += 1;

        }

        return;

    }


    for (int i = 0; i < CurrentNode->array_stored_elements + 1; i++)
    {

        BplusTreeCheckBalance_Tool(CurrentNode->NodeLinkArray[i], node_height + 1, BplusTree_height, unbalanced_nodes);

    }


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool BplusTreeCheckLeafLinkNodes(

        BplusTree BplusTree,
        int* improperly_structured_nodes

)
{


    if (BplusTree.Root == NULL)
    {

        printf("Bplus-tree is empty\n\n");

        return 1;

    }


    *improperly_structured_nodes = 0;
    BplusTreeCheckLeafLinkNodes_Tool(BplusTree.Root, improperly_structured_nodes);


    return 0;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BplusTreeCheckLeafLinkNodes_Tool(

        BplusTreeNode CurrentNode,
        int* improperly_structured_nodes

)
{


    if (CurrentNode->leaf_type)
    {

        if (CurrentNode->NodeLinkArray != NULL)
        {

            *improperly_structured_nodes += 1;

        }

        return;

    }


    for (int i = 0; i < CurrentNode->array_stored_elements + 1; i++)
    {

        BplusTreeCheckLeafLinkNodes_Tool(CurrentNode->NodeLinkArray[i], improperly_structured_nodes);

    }


}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BplusTreeData BplusTreeSearch_NodeType_ByRecord(

        BplusTree BplusTree,
        BplusTreePrimaryKey Record_PrimaryKey,
        int* record_node_type_flag,

        BplusTreePrimaryKey (*Get_Record_PrimaryKey)(BplusTreeData)

)
{


    if (BplusTree.Root == NULL)
    {

        //printf("Bplus-tree is empty\n\n");

        return NULL;

    }


    *record_node_type_flag = 0;

    return BplusTreeSearch_NodeType_ByRecord_Tool(BplusTree.Root, Record_PrimaryKey, record_node_type_flag, Get_Record_PrimaryKey);


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BplusTreeData BplusTreeSearch_NodeType_ByRecord_Tool(

        BplusTreeNode CurrentNode,
        BplusTreePrimaryKey Record_PrimaryKey,
        int* record_node_type_flag,

        BplusTreePrimaryKey (*Get_Record_PrimaryKey)(BplusTreeData)

)
{


    int record_position = CurrentNode->array_stored_elements;

    record_position = SearchBplusTreeNode_Record_ByPrimaryKey( CurrentNode->array_stored_elements,
                                                               CurrentNode->RecordDataArray,
                                                               Record_PrimaryKey,
                                                               Get_Record_PrimaryKey );


    if ( (record_position < CurrentNode->array_stored_elements)
         && (Get_Record_PrimaryKey(CurrentNode->RecordDataArray[record_position]) == Record_PrimaryKey) )
    {

        *record_node_type_flag += 1;


        if (CurrentNode->leaf_type)
        {

            return CurrentNode->RecordDataArray[record_position];

        }

    }


    if (!CurrentNode->leaf_type)
    {

        return BplusTreeSearch_NodeType_ByRecord_Tool( CurrentNode->NodeLinkArray[record_position],
                                                       Record_PrimaryKey,
                                                       record_node_type_flag,
                                                       Get_Record_PrimaryKey );

    }


    return NULL;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool BplusTreeCheckInternalNode_ListLinks(

        BplusTree BplusTree,
        int* improperly_structured_nodes

)
{


    if (BplusTree.Root == NULL)
    {

        printf("Bplus-tree is empty\n\n");

        return 1;

    }


    *improperly_structured_nodes = 0;
    BplusTreeCheckInternalNode_ListLinks_Tool(BplusTree.Root, improperly_structured_nodes);


    return 0;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BplusTreeCheckInternalNode_ListLinks_Tool(

        BplusTreeNode CurrentNode,
        int* improperly_structured_nodes

)
{


    if (CurrentNode->leaf_type)
    {

        return;

    }


    if (CurrentNode->NextLeafNode != NULL || CurrentNode->PreviousLeafNode != NULL)
    {

        *improperly_structured_nodes += 1;

    }


    for (int i = 0; i < CurrentNode->array_stored_elements + 1; i++)
    {

        BplusTreeCheckInternalNode_ListLinks_Tool(CurrentNode->NodeLinkArray[i], improperly_structured_nodes);

    }


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool BplusTreeDeleteData(

        BplusTree* BplusTree,
        BplusTreePrimaryKey PrimaryKey,
        BplusTreeData* DeletedRecordData,

        BplusTreePrimaryKey (*Get_Record_PrimaryKey)(BplusTreeData)

)
{


    if (BplusTree->Root == NULL)
    {

        //printf("Bplus-tree is empty\n\n");

        *DeletedRecordData = NULL;

        return 1;

    }


    if (BplusTree->Root->leaf_type)
    {

        int record_position = SearchBplusTreeNode_Record_ByPrimaryKey( BplusTree->Root->array_stored_elements,
                                                                       BplusTree->Root->RecordDataArray,
                                                                       PrimaryKey,
                                                                       Get_Record_PrimaryKey );


        if ( (record_position < BplusTree->Root->array_stored_elements)
             && (Get_Record_PrimaryKey(BplusTree->Root->RecordDataArray[record_position]) == PrimaryKey) )
        {

            *DeletedRecordData = BplusTree->Root->RecordDataArray[record_position];


            if (BplusTree->Root->array_stored_elements == 1)
            {

                free(BplusTree->Root->RecordDataArray);
                free(BplusTree->Root);

                InitializeBplustree(BplusTree, BplusTree->node_size);

                return 0;

            }


            BplusTree_ReplaceRecord(BplusTree->Root, record_position, BplusTree->node_size);


            BplusTree->size -= 1;

            return 0;

        }


        //printf("BplusTreePrimaryKey : %d not found\n\n", PrimaryKey);

        *DeletedRecordData = NULL;

        return 1;


    }


    bool balancing_flag = 0;
    bool internal_record_deletion_flag = 0;
    bool record_existence_flag = 0;


    BplusTree->Root = BplusTreeDeleteNode( BplusTree->Root,
                                           PrimaryKey,
                                           DeletedRecordData,
                                           &(BplusTree->Root->RecordDataArray[0]),
                                           BplusTree->node_size,
                                           &balancing_flag,
                                           &internal_record_deletion_flag,
                                           &record_existence_flag,
                                           Get_Record_PrimaryKey );


    if (record_existence_flag == 0)
    {

        BplusTree->size -= 1;

    }
    else {

        return 1;

    }


    if (balancing_flag == 1)
    {

        BplusTree->height -= 1;

    }


    return 0;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BplusTreeNode BplusTreeDeleteNode(

        BplusTreeNode CurrentNode,
        BplusTreePrimaryKey PrimaryKey,
        BplusTreeData* DeletedRecordData,
        BplusTreeData* internal_record_node_position,
        int node_size,
        bool* balancing_flag,
        bool* internal_record_deletion_flag,
        bool* record_existence_flag,

        BplusTreePrimaryKey (*Get_Record_PrimaryKey)(BplusTreeData)

)
{


    int record_position = SearchBplusTreeNode_Record_ByPrimaryKey( CurrentNode->array_stored_elements,
                                                                   CurrentNode->RecordDataArray,
                                                                   PrimaryKey,
                                                                   Get_Record_PrimaryKey );


    if ( (record_position < CurrentNode->array_stored_elements)
         && (Get_Record_PrimaryKey(CurrentNode->RecordDataArray[record_position]) == PrimaryKey) )
    {

        *DeletedRecordData = CurrentNode->RecordDataArray[record_position];
        internal_record_node_position = &(CurrentNode->RecordDataArray[record_position]);

        PrimaryKey = Get_Record_PrimaryKey(CurrentNode->RecordDataArray[record_position]);

        *internal_record_deletion_flag = 1;

    }


    if (!CurrentNode->NodeLinkArray[record_position]->leaf_type)
    {

        BplusTreeDeleteNode( CurrentNode->NodeLinkArray[record_position],
                             PrimaryKey,
                             DeletedRecordData,
                             internal_record_node_position,
                             node_size, balancing_flag,
                             internal_record_deletion_flag,
                             record_existence_flag,
                             Get_Record_PrimaryKey );


        if (*balancing_flag == 1)
        {

            BplusTreeDelete_NonLeafNode(CurrentNode, record_position, node_size, balancing_flag);

        }
        else {

            *balancing_flag = 0;

        }


        return CurrentNode;

    }


    BplusTreeDelete_LeafNode( CurrentNode,
                              PrimaryKey,
                              DeletedRecordData,
                              record_position,
                              internal_record_node_position,
                              node_size,
                              balancing_flag,
                              *internal_record_deletion_flag,
                              record_existence_flag,
                              Get_Record_PrimaryKey );

    return CurrentNode;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BplusTreeDelete_NonLeafNode(

        BplusTreeNode CurrentNode,
        int record_position,
        int node_size,
        bool* balancing_flag

)
{


    if (record_position > 0)
    {

        BplusTreeNode LeftNode = CurrentNode->NodeLinkArray[record_position - 1];


        if (CurrentNode->array_stored_elements == 1)
        {

            if (LeftNode->array_stored_elements == 1)
            {

                BplusTree_MergeLeftNodeRecursive(CurrentNode, LeftNode, node_size);

                *balancing_flag = 1;

            }
            else {

                BplusTree_ReplaceLeftNodeRecursive(CurrentNode, LeftNode, node_size);

                *balancing_flag = 0;

            }

        }
        else {

            if (LeftNode->array_stored_elements > 1)
            {

                BplusTree_ReplaceMultipleLeftNodeRecursive(CurrentNode, LeftNode, record_position, node_size);

                *balancing_flag = 0;

            }
            else {

                BplusTree_ReplaceSingleLeftNodeRecursive(CurrentNode, LeftNode, record_position, node_size);

                *balancing_flag = 0;

            }

        }

    }
    else {

        BplusTreeNode RightNode = CurrentNode->NodeLinkArray[record_position + 1];


        if (CurrentNode->array_stored_elements == 1)
        {

            if (RightNode->array_stored_elements == 1)
            {

                BplusTree_MergeRightNodeRecursive(CurrentNode, RightNode, node_size);

                *balancing_flag = 1;

            }
            else {

                BplusTree_ReplaceRightNodeRecursive(CurrentNode, RightNode, node_size);

                *balancing_flag = 0;

            }

        }
        else {

            if (RightNode->array_stored_elements > 1)
            {

                BplusTree_ReplaceMultipleRightNodeRecursive(CurrentNode, RightNode, record_position, node_size);

                *balancing_flag = 0;

            }
            else {

                BplusTree_ReplaceSingleRightNodeRecursive(CurrentNode, RightNode, record_position, node_size);

                *balancing_flag = 0;

            }

        }

    }


    return;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BplusTreeDelete_LeafNode(

        BplusTreeNode CurrentNode,
        BplusTreePrimaryKey PrimaryKey,
        BplusTreeData* DeletedRecordData,
        int record_position,
        BplusTreeData* internal_record_node_position,
        int node_size,
        bool* balancing_flag,
        bool internal_record_deletion_flag,
        bool* record_existence_flag,

        BplusTreePrimaryKey (*Get_Record_PrimaryKey)(BplusTreeData)

)
{


    BplusTreeNode NextLevelNode = CurrentNode->NodeLinkArray[record_position];

    int deletion_position = SearchBplusTreeNode_Record_ByPrimaryKey( NextLevelNode->array_stored_elements,
                                                                     NextLevelNode->RecordDataArray,
                                                                     PrimaryKey,
                                                                     Get_Record_PrimaryKey );


    if ( (deletion_position < NextLevelNode->array_stored_elements)
         && (Get_Record_PrimaryKey(NextLevelNode->RecordDataArray[deletion_position]) == PrimaryKey) )
    {

        if (internal_record_deletion_flag == 0)
        {

            *DeletedRecordData = NextLevelNode->RecordDataArray[deletion_position];

        }


        if (NextLevelNode->array_stored_elements > 1)
        {

            if (internal_record_deletion_flag == 1)
            {

                *internal_record_node_position = NextLevelNode->RecordDataArray[deletion_position - 1];

            }


            BplusTree_ReplaceRecord(NextLevelNode, deletion_position, node_size);

            *balancing_flag = 0;

            return;

        }


        if (CurrentNode->array_stored_elements == 1)
        {

            if ((CurrentNode->NodeLinkArray[0]->array_stored_elements == 1) && (CurrentNode->NodeLinkArray[1]->array_stored_elements == 1))
            {

                if (record_position == 0)
                {

                    BplusTree_MergeRightNode(CurrentNode);

                    *balancing_flag = 1;

                }


                if (record_position == 1)
                {

                    if (internal_record_deletion_flag == 1)
                    {

                        *internal_record_node_position = CurrentNode->RecordDataArray[0];

                    }


                    BplusTree_MergeLeftNode(CurrentNode);

                    *balancing_flag = 1;

                }


                return;

            }

            if ((CurrentNode->NodeLinkArray[0]->array_stored_elements > 1) || (CurrentNode->NodeLinkArray[1]->array_stored_elements > 1))
            {

                if (CurrentNode->NodeLinkArray[1]->array_stored_elements > 1)
                {

                    *internal_record_node_position = CurrentNode->NodeLinkArray[1]->RecordDataArray[0];

                    BplusTree_RebalanceRightNode(CurrentNode, NextLevelNode, node_size);

                }
                else {

                    if (internal_record_deletion_flag == 1)
                    {

                        *internal_record_node_position = CurrentNode->RecordDataArray[0];

                    }


                    BplusTree_RebalanceLeftNode(CurrentNode, NextLevelNode, node_size);

                }


                *balancing_flag = 0;

                return;

            }


        }


        if (CurrentNode->array_stored_elements > 1)
        {

            if (record_position == CurrentNode->array_stored_elements)
            {

                if (internal_record_deletion_flag == 1)
                {

                    *internal_record_node_position = CurrentNode->RecordDataArray[CurrentNode->array_stored_elements - 1];

                }

                BplusTree_ReplaceRecord_Left_to_Right(CurrentNode, NextLevelNode, record_position, node_size);

            }
            else {

                BplusTree_ReplaceRecord_Right_to_Left(CurrentNode, NextLevelNode, record_position, node_size);

            }


            *balancing_flag = 0;

            return;

        }


    }


    //printf("BplusTreePrimaryKey : %d not found\n\n", PrimaryKey);

    *DeletedRecordData = NULL;

    *record_existence_flag = 1;


    return;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BplusTree_ReplaceRecord(

        BplusTreeNode CurrentNode,
        int record_position,
        int node_size

)
{


    for (int i = record_position; i < (CurrentNode->array_stored_elements - 1); i++)
    {

        CurrentNode->RecordDataArray[i] = CurrentNode->RecordDataArray[i + 1];

    }


    CurrentNode->array_stored_elements--;


    if (CurrentNode->array_available_capacity == node_size && (CurrentNode->array_stored_elements + 1) == (node_size / 2))
    {

        CurrentNode->RecordDataArray = (BplusTreeData*)(realloc(CurrentNode->RecordDataArray, (node_size / 2) * sizeof(BplusTreeData)));
        CurrentNode->array_available_capacity = node_size / 2;

    }


    return;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BplusTree_MergeLeftNode(

        BplusTreeNode CurrentNode

)
{


    if (CurrentNode->NodeLinkArray[1]->NextLeafNode == NULL)
    {

        CurrentNode->NextLeafNode = NULL;
        CurrentNode->PreviousLeafNode = CurrentNode->NodeLinkArray[0]->PreviousLeafNode;

        if (CurrentNode->NodeLinkArray[0]->PreviousLeafNode != NULL)
        {

            CurrentNode->NodeLinkArray[0]->PreviousLeafNode->NextLeafNode = CurrentNode;

        }

    }else{

        CurrentNode->NextLeafNode = CurrentNode->NodeLinkArray[1]->NextLeafNode;
        CurrentNode->NodeLinkArray[1]->NextLeafNode->PreviousLeafNode = CurrentNode;
        CurrentNode->PreviousLeafNode = CurrentNode->NodeLinkArray[0]->PreviousLeafNode;


        if (CurrentNode->NodeLinkArray[0]->PreviousLeafNode != NULL)
        {

            CurrentNode->NodeLinkArray[0]->PreviousLeafNode->NextLeafNode = CurrentNode;

        }

    }


    CurrentNode->leaf_type = 1;

    free(CurrentNode->NodeLinkArray[0]->RecordDataArray);
    free(CurrentNode->NodeLinkArray[0]);

    free(CurrentNode->NodeLinkArray[1]->RecordDataArray);
    free(CurrentNode->NodeLinkArray[1]);

    free(CurrentNode->NodeLinkArray);
    CurrentNode->NodeLinkArray = NULL;


    return;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BplusTree_MergeRightNode(

        BplusTreeNode CurrentNode

)
{


    if (CurrentNode->NodeLinkArray[1]->NextLeafNode == NULL)
    {

        CurrentNode->NextLeafNode = NULL;
        CurrentNode->PreviousLeafNode = CurrentNode->NodeLinkArray[0]->PreviousLeafNode;


        if (CurrentNode->NodeLinkArray[0]->PreviousLeafNode != NULL)
        {

            CurrentNode->NodeLinkArray[0]->PreviousLeafNode->NextLeafNode = CurrentNode;

        }

    }else{

        CurrentNode->NextLeafNode = CurrentNode->NodeLinkArray[1]->NextLeafNode;
        CurrentNode->NodeLinkArray[1]->NextLeafNode->PreviousLeafNode = CurrentNode;
        CurrentNode->PreviousLeafNode = CurrentNode->NodeLinkArray[0]->PreviousLeafNode;


        if (CurrentNode->NodeLinkArray[0]->PreviousLeafNode != NULL)
        {

            CurrentNode->NodeLinkArray[0]->PreviousLeafNode->NextLeafNode = CurrentNode;

        }

    }


    CurrentNode->RecordDataArray[0] = CurrentNode->NodeLinkArray[1]->RecordDataArray[0];
    CurrentNode->leaf_type = 1;

    free(CurrentNode->NodeLinkArray[0]->RecordDataArray);
    free(CurrentNode->NodeLinkArray[0]);

    free(CurrentNode->NodeLinkArray[1]->RecordDataArray);
    free(CurrentNode->NodeLinkArray[1]);

    free(CurrentNode->NodeLinkArray);
    CurrentNode->NodeLinkArray = NULL;


    return;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BplusTree_RebalanceLeftNode(

        BplusTreeNode CurrentNode,
        BplusTreeNode NextLevelNode,
        int node_size

)
{


    NextLevelNode->RecordDataArray[0] = CurrentNode->RecordDataArray[0];
    CurrentNode->RecordDataArray[0] = CurrentNode->NodeLinkArray[0]->RecordDataArray[CurrentNode->NodeLinkArray[0]->array_stored_elements - 2];

    CurrentNode->NodeLinkArray[0]->array_stored_elements--;


    if (CurrentNode->NodeLinkArray[0]->array_available_capacity == node_size && (CurrentNode->NodeLinkArray[0]->array_stored_elements + 1) == (node_size / 2))
    {

        CurrentNode->NodeLinkArray[0]->RecordDataArray = (BplusTreeData*)(realloc(CurrentNode->NodeLinkArray[0]->RecordDataArray, (node_size / 2) * sizeof(BplusTreeData)));
        CurrentNode->NodeLinkArray[0]->array_available_capacity = node_size / 2;

    }


    return;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BplusTree_RebalanceRightNode(

        BplusTreeNode CurrentNode,
        BplusTreeNode NextLevelNode,
        int node_size

)
{


    NextLevelNode->RecordDataArray[0] = CurrentNode->NodeLinkArray[1]->RecordDataArray[0];


    for (int i = 0; i < (CurrentNode->NodeLinkArray[1]->array_stored_elements - 1); i++)
    {

        CurrentNode->NodeLinkArray[1]->RecordDataArray[i] = CurrentNode->NodeLinkArray[1]->RecordDataArray[i + 1];

    }


    CurrentNode->NodeLinkArray[1]->array_stored_elements--;


    if (CurrentNode->NodeLinkArray[1]->array_available_capacity == node_size && (CurrentNode->NodeLinkArray[1]->array_stored_elements + 1) == (node_size / 2))
    {

        CurrentNode->NodeLinkArray[1]->RecordDataArray = (BplusTreeData*)(realloc(CurrentNode->NodeLinkArray[1]->RecordDataArray, (node_size / 2) * sizeof(BplusTreeData)));
        CurrentNode->NodeLinkArray[1]->array_available_capacity = node_size / 2;

    }


    return;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BplusTree_ReplaceRecord_Right_to_Left(

        BplusTreeNode CurrentNode,
        BplusTreeNode NextLevelNode,
        int record_position,
        int node_size

)
{


    BplusTreeNode NextNode = CurrentNode->NodeLinkArray[record_position + 1];


    if (CurrentNode->NodeLinkArray[record_position]->PreviousLeafNode == NULL)
    {

        NextNode->PreviousLeafNode = NULL;

    }else {

        CurrentNode->NodeLinkArray[record_position]->PreviousLeafNode->NextLeafNode = NextNode;
        NextNode->PreviousLeafNode = CurrentNode->NodeLinkArray[record_position]->PreviousLeafNode;

    }


    free(CurrentNode->NodeLinkArray[record_position]->RecordDataArray);
    free(CurrentNode->NodeLinkArray[record_position]);


    for (int i = record_position; i < (CurrentNode->array_stored_elements - 1); i++)
    {

        CurrentNode->RecordDataArray[i] = CurrentNode->RecordDataArray[i + 1];

    }


    for (int i = record_position; i <= (CurrentNode->array_stored_elements - 1); i++)
    {

        CurrentNode->NodeLinkArray[i] = CurrentNode->NodeLinkArray[i + 1];

    }


    if (record_position == (CurrentNode->array_stored_elements - 1))
    {

        CurrentNode->NodeLinkArray[record_position] = NextNode;

    }


    CurrentNode->array_stored_elements--;


    if (CurrentNode->array_available_capacity == node_size && (CurrentNode->array_stored_elements + 1) == (node_size / 2))
    {

        CurrentNode->RecordDataArray = (BplusTreeData*)(realloc(CurrentNode->RecordDataArray, (node_size / 2) * sizeof(BplusTreeData)));
        CurrentNode->NodeLinkArray = (BplusTreeNode*)(realloc(CurrentNode->NodeLinkArray, (node_size / 2 + 1) * sizeof(BplusTreeNode)));
        CurrentNode->array_available_capacity = node_size / 2;

    }


    return;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BplusTree_ReplaceRecord_Left_to_Right(

        BplusTreeNode CurrentNode,
        BplusTreeNode NextLevelNode,
        int record_position,
        int node_size

)
{


    BplusTreeNode PreviousNode = CurrentNode->NodeLinkArray[record_position - 1];


    if (CurrentNode->NodeLinkArray[record_position]->NextLeafNode == NULL)
    {

        PreviousNode->NextLeafNode = NULL;

    }else {

        PreviousNode->NextLeafNode = CurrentNode->NodeLinkArray[record_position]->NextLeafNode;
        CurrentNode->NodeLinkArray[record_position]->NextLeafNode->PreviousLeafNode = PreviousNode;

    }


    free(CurrentNode->NodeLinkArray[record_position]->RecordDataArray);
    free(CurrentNode->NodeLinkArray[record_position]);


    CurrentNode->array_stored_elements--;


    if (CurrentNode->array_available_capacity == node_size && (CurrentNode->array_stored_elements + 1) == (node_size / 2))
    {

        CurrentNode->RecordDataArray = (BplusTreeData*)(realloc(CurrentNode->RecordDataArray, (node_size / 2) * sizeof(BplusTreeData)));
        CurrentNode->NodeLinkArray = (BplusTreeNode*)(realloc(CurrentNode->NodeLinkArray, (node_size / 2 + 1) * sizeof(BplusTreeNode)));
        CurrentNode->array_available_capacity = node_size / 2;

    }


    return;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BplusTree_MergeLeftNodeRecursive(

        BplusTreeNode CurrentNode,
        BplusTreeNode LeftNode,
        int node_size

)
{


    CurrentNode->array_stored_elements++;


    if (CurrentNode->array_available_capacity == (node_size / 2) && (CurrentNode->array_stored_elements - 1) == (node_size / 2))
    {

        CurrentNode->RecordDataArray = (BplusTreeData*)(realloc(CurrentNode->RecordDataArray, node_size * sizeof(BplusTreeData)));
        CurrentNode->NodeLinkArray = (BplusTreeNode*)(realloc(CurrentNode->NodeLinkArray, (node_size + 1) * sizeof(BplusTreeNode)));
        CurrentNode->array_available_capacity = node_size;

    }


    CurrentNode->RecordDataArray[1] = CurrentNode->RecordDataArray[0];
    CurrentNode->RecordDataArray[0] = LeftNode->RecordDataArray[0];

    CurrentNode->NodeLinkArray[2] = CurrentNode->NodeLinkArray[1];
    CurrentNode->NodeLinkArray[1] = LeftNode->NodeLinkArray[1];
    CurrentNode->NodeLinkArray[0] = LeftNode->NodeLinkArray[0];

    free(LeftNode->RecordDataArray);
    free(LeftNode->NodeLinkArray);
    free(LeftNode);

    return;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BplusTree_MergeRightNodeRecursive(

        BplusTreeNode CurrentNode,
        BplusTreeNode RightNode,
        int node_size

)
{


    CurrentNode->array_stored_elements++;


    if (CurrentNode->array_available_capacity == (node_size / 2) && (CurrentNode->array_stored_elements - 1) == (node_size / 2))
    {

        CurrentNode->RecordDataArray = (BplusTreeData*)(realloc(CurrentNode->RecordDataArray, node_size * sizeof(BplusTreeData)));
        CurrentNode->NodeLinkArray = (BplusTreeNode*)(realloc(CurrentNode->NodeLinkArray, (node_size + 1) * sizeof(BplusTreeNode)));
        CurrentNode->array_available_capacity = node_size;

    }


    CurrentNode->RecordDataArray[1] = RightNode->RecordDataArray[0];

    CurrentNode->NodeLinkArray[1] = RightNode->NodeLinkArray[0];
    CurrentNode->NodeLinkArray[2] = RightNode->NodeLinkArray[1];

    free(RightNode->RecordDataArray);
    free(RightNode->NodeLinkArray);
    free(RightNode);

    return;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BplusTree_ReplaceLeftNodeRecursive(

        BplusTreeNode CurrentNode,
        BplusTreeNode LeftNode,
        int node_size

)
{


    BplusTreeNode NewNode = (BplusTreeNode)(malloc(sizeof(struct BplusTreeNode)));

    NewNode->array_stored_elements = 1;
    NewNode->leaf_type = 0;
    NewNode->NextLeafNode = NULL;
    NewNode->PreviousLeafNode = NULL;

    NewNode->RecordDataArray = (BplusTreeData*)(malloc((node_size / 2) * sizeof(BplusTreeData)));
    NewNode->NodeLinkArray = (BplusTreeNode*)(malloc((node_size / 2 + 1) * sizeof(BplusTreeNode)));
    NewNode->array_available_capacity = node_size / 2;

    NewNode->RecordDataArray[0] = CurrentNode->RecordDataArray[0];
    NewNode->NodeLinkArray[0] = LeftNode->NodeLinkArray[LeftNode->array_stored_elements];
    NewNode->NodeLinkArray[1] = CurrentNode->NodeLinkArray[1];
    CurrentNode->NodeLinkArray[1] = NewNode;
    CurrentNode->RecordDataArray[0] = LeftNode->RecordDataArray[LeftNode->array_stored_elements - 1];

    LeftNode->array_stored_elements--;


    if (LeftNode->array_available_capacity == node_size && (LeftNode->array_stored_elements + 1) == (node_size / 2))
    {

        LeftNode->RecordDataArray = (BplusTreeData*)(realloc(LeftNode->RecordDataArray, (node_size / 2) * sizeof(BplusTreeData)));
        LeftNode->NodeLinkArray = (BplusTreeNode*)(realloc(LeftNode->NodeLinkArray, (node_size / 2 + 1) * sizeof(BplusTreeNode)));
        LeftNode->array_available_capacity = node_size / 2;

    }


    return;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BplusTree_ReplaceRightNodeRecursive(

        BplusTreeNode CurrentNode,
        BplusTreeNode RightNode,
        int node_size

)
{


    BplusTreeNode NewNode = (BplusTreeNode)(malloc(sizeof(struct BplusTreeNode)));

    NewNode->array_stored_elements = 1;
    NewNode->leaf_type = 0;
    NewNode->NextLeafNode = NULL;
    NewNode->PreviousLeafNode = NULL;

    NewNode->RecordDataArray = (BplusTreeData*)(malloc((node_size / 2) * sizeof(BplusTreeData)));
    NewNode->NodeLinkArray = (BplusTreeNode*)(malloc((node_size / 2 + 1) * sizeof(BplusTreeNode)));
    NewNode->array_available_capacity = node_size / 2;

    NewNode->RecordDataArray[0] = CurrentNode->RecordDataArray[0];
    NewNode->NodeLinkArray[0] = CurrentNode->NodeLinkArray[0];
    NewNode->NodeLinkArray[1] = RightNode->NodeLinkArray[0];
    CurrentNode->NodeLinkArray[0] = NewNode;
    CurrentNode->RecordDataArray[0] = RightNode->RecordDataArray[0];


    for (int i = 0; i < (RightNode->array_stored_elements - 1); i++)
    {

        RightNode->RecordDataArray[i] = RightNode->RecordDataArray[i + 1];

    }


    for (int i = 0; i < RightNode->array_stored_elements; i++)
    {

        RightNode->NodeLinkArray[i] = RightNode->NodeLinkArray[i + 1];

    }


    RightNode->array_stored_elements--;


    if (RightNode->array_available_capacity == node_size && (RightNode->array_stored_elements + 1) == (node_size / 2))
    {

        RightNode->RecordDataArray = (BplusTreeData*)(realloc(RightNode->RecordDataArray, (node_size / 2) * sizeof(BplusTreeData)));
        RightNode->NodeLinkArray = (BplusTreeNode*)(realloc(RightNode->NodeLinkArray, (node_size / 2 + 1) * sizeof(BplusTreeNode)));
        RightNode->array_available_capacity = node_size / 2;

    }


    return;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BplusTree_ReplaceMultipleLeftNodeRecursive(

        BplusTreeNode CurrentNode,
        BplusTreeNode LeftNode,
        int record_position,
        int node_size

)
{


    BplusTreeNode NewNode = (BplusTreeNode)(malloc(sizeof(struct BplusTreeNode)));

    NewNode->array_stored_elements = 1;
    NewNode->leaf_type = 0;
    NewNode->NextLeafNode = NULL;
    NewNode->PreviousLeafNode = NULL;

    NewNode->RecordDataArray = (BplusTreeData*)(malloc((node_size / 2) * sizeof(BplusTreeData)));
    NewNode->NodeLinkArray = (BplusTreeNode*)(malloc((node_size / 2 + 1) * sizeof(BplusTreeNode)));
    NewNode->array_available_capacity = node_size / 2;

    NewNode->RecordDataArray[0] = CurrentNode->RecordDataArray[record_position - 1];
    NewNode->NodeLinkArray[0] = LeftNode->NodeLinkArray[LeftNode->array_stored_elements];
    NewNode->NodeLinkArray[1] = CurrentNode->NodeLinkArray[record_position];
    CurrentNode->NodeLinkArray[record_position] = NewNode;
    CurrentNode->RecordDataArray[record_position - 1] = LeftNode->RecordDataArray[LeftNode->array_stored_elements - 1];

    LeftNode->array_stored_elements--;


    if (LeftNode->array_available_capacity == node_size && (LeftNode->array_stored_elements + 1) == (node_size / 2))
    {

        LeftNode->RecordDataArray = (BplusTreeData*)(realloc(LeftNode->RecordDataArray, (node_size / 2) * sizeof(BplusTreeData)));
        LeftNode->NodeLinkArray = (BplusTreeNode*)(realloc(LeftNode->NodeLinkArray, (node_size / 2 + 1) * sizeof(BplusTreeNode)));
        LeftNode->array_available_capacity = node_size / 2;

    }


    return;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BplusTree_ReplaceSingleLeftNodeRecursive(

        BplusTreeNode CurrentNode,
        BplusTreeNode LeftNode,
        int record_position,
        int node_size

)
{


    LeftNode->array_stored_elements++;


    if (LeftNode->array_available_capacity == (node_size / 2) && (LeftNode->array_stored_elements - 1) == (node_size / 2))
    {

        LeftNode->RecordDataArray = (BplusTreeData*)(realloc(LeftNode->RecordDataArray, node_size * sizeof(BplusTreeData)));
        LeftNode->NodeLinkArray = (BplusTreeNode*)(realloc(LeftNode->NodeLinkArray, (node_size + 1) * sizeof(BplusTreeNode)));
        LeftNode->array_available_capacity = node_size;

    }


    LeftNode->RecordDataArray[LeftNode->array_stored_elements - 1] = CurrentNode->RecordDataArray[record_position - 1];
    LeftNode->NodeLinkArray[LeftNode->array_stored_elements] = CurrentNode->NodeLinkArray[record_position];


    for (int i = record_position - 1; i < (CurrentNode->array_stored_elements - 1); i++)
    {

        CurrentNode->RecordDataArray[i] = CurrentNode->RecordDataArray[i + 1];
        CurrentNode->NodeLinkArray[i + 1] = CurrentNode->NodeLinkArray[i + 2];

    }


    CurrentNode->array_stored_elements--;


    if (CurrentNode->array_available_capacity == node_size && (CurrentNode->array_stored_elements + 1) == (node_size / 2))
    {

        CurrentNode->RecordDataArray = (BplusTreeData*)(realloc(CurrentNode->RecordDataArray, (node_size / 2) * sizeof(BplusTreeData)));
        CurrentNode->NodeLinkArray = (BplusTreeNode*)(realloc(CurrentNode->NodeLinkArray, (node_size / 2 + 1) * sizeof(BplusTreeNode)));
        CurrentNode->array_available_capacity = node_size / 2;

    }


    return;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BplusTree_ReplaceMultipleRightNodeRecursive(

        BplusTreeNode CurrentNode,
        BplusTreeNode RightNode,
        int record_position,
        int node_size

)
{


    BplusTreeNode NewNode = (BplusTreeNode)(malloc(sizeof(struct BplusTreeNode)));

    NewNode->array_stored_elements = 1;
    NewNode->leaf_type = 0;
    NewNode->NextLeafNode = NULL;
    NewNode->PreviousLeafNode = NULL;

    NewNode->RecordDataArray = (BplusTreeData*)(malloc((node_size / 2) * sizeof(BplusTreeData)));
    NewNode->NodeLinkArray = (BplusTreeNode*)(malloc((node_size / 2 + 1) * sizeof(BplusTreeNode)));
    NewNode->array_available_capacity = node_size / 2;

    NewNode->RecordDataArray[0] = CurrentNode->RecordDataArray[record_position];
    NewNode->NodeLinkArray[0] = CurrentNode->NodeLinkArray[record_position];
    NewNode->NodeLinkArray[1] = RightNode->NodeLinkArray[0];
    CurrentNode->NodeLinkArray[record_position] = NewNode;
    CurrentNode->RecordDataArray[record_position] = RightNode->RecordDataArray[0];


    for (int i = 0; i < (RightNode->array_stored_elements - 1); i++)
    {

        RightNode->RecordDataArray[i] = RightNode->RecordDataArray[i + 1];

    }


    for (int i = 0; i < RightNode->array_stored_elements; i++)
    {

        RightNode->NodeLinkArray[i] = RightNode->NodeLinkArray[i + 1];

    }


    RightNode->array_stored_elements--;


    if (RightNode->array_available_capacity == node_size && (RightNode->array_stored_elements + 1) == (node_size / 2))
    {

        RightNode->RecordDataArray = (BplusTreeData*)(realloc(RightNode->RecordDataArray, (node_size / 2) * sizeof(BplusTreeData)));
        RightNode->NodeLinkArray = (BplusTreeNode*)(realloc(RightNode->NodeLinkArray, (node_size / 2 + 1) * sizeof(BplusTreeNode)));
        RightNode->array_available_capacity = node_size / 2;

    }


    return;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BplusTree_ReplaceSingleRightNodeRecursive(

        BplusTreeNode CurrentNode,
        BplusTreeNode RightNode,
        int record_position,
        int node_size

)
{


    RightNode->array_stored_elements++;


    if (RightNode->array_available_capacity == (node_size / 2) && (RightNode->array_stored_elements - 1) == (node_size / 2))
    {

        RightNode->RecordDataArray = (BplusTreeData*)(realloc(RightNode->RecordDataArray, node_size * sizeof(BplusTreeData)));
        RightNode->NodeLinkArray = (BplusTreeNode*)(realloc(RightNode->NodeLinkArray, (node_size + 1) * sizeof(BplusTreeNode)));
        RightNode->array_available_capacity = node_size;

    }


    RightNode->RecordDataArray[1] = RightNode->RecordDataArray[0];
    RightNode->RecordDataArray[0] = CurrentNode->RecordDataArray[record_position];


    for (int i = RightNode->array_stored_elements; i > 0; i--)
    {

        RightNode->NodeLinkArray[i] = RightNode->NodeLinkArray[i - 1];

    }


    RightNode->NodeLinkArray[0] = CurrentNode->NodeLinkArray[record_position];


    for (int i = record_position; i < (CurrentNode->array_stored_elements - 1); i++)
    {

        CurrentNode->RecordDataArray[i] = CurrentNode->RecordDataArray[i + 1];

    }


    for (int i = record_position; i < CurrentNode->array_stored_elements; i++)
    {

        CurrentNode->NodeLinkArray[i] = CurrentNode->NodeLinkArray[i + 1];

    }


    CurrentNode->array_stored_elements--;


    if (CurrentNode->array_available_capacity == node_size && (CurrentNode->array_stored_elements + 1) == (node_size / 2))
    {

        CurrentNode->RecordDataArray = (BplusTreeData*)(realloc(CurrentNode->RecordDataArray, (node_size / 2) * sizeof(BplusTreeData)));
        CurrentNode->NodeLinkArray = (BplusTreeNode*)(realloc(CurrentNode->NodeLinkArray, (node_size / 2 + 1) * sizeof(BplusTreeNode)));
        CurrentNode->array_available_capacity = node_size / 2;

    }


    return;


}

