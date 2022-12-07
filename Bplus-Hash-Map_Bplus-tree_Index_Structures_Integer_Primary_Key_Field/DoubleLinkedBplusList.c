
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "BplusTree.h"



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void Initialize_BplusList(

        struct DoubleLinkedBplusList* LinkedList

)
{


    LinkedList->List_HeadNode = NULL;
    LinkedList->List_TailNode = NULL;
    LinkedList->List_Size = 0;

    return;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


struct BplusListNode* Create_BplusListNode(

        BplusListData RecordData

)
{


    struct BplusListNode* NewListNode = (BplusListNode*)(malloc(sizeof(struct BplusListNode)));

    NewListNode->RecordData = RecordData;
    NewListNode->List_NextNode = NULL;
    NewListNode->List_PrevNode = NULL;

    return NewListNode;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void InsertBplusListNode_First(

        struct DoubleLinkedBplusList* LinkedList,
        BplusListData RecordData

)
{


    struct BplusListNode* TempNode = Create_BplusListNode(RecordData);


    if (LinkedList->List_HeadNode == NULL && LinkedList->List_TailNode == NULL)
    {

        LinkedList->List_HeadNode = TempNode;
        LinkedList->List_TailNode = TempNode;

        LinkedList->List_Size++;

        return;

    }


    TempNode->List_NextNode = LinkedList->List_HeadNode;
    LinkedList->List_HeadNode->List_PrevNode = TempNode;
    LinkedList->List_HeadNode = TempNode;

    LinkedList->List_Size++;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void InsertBplusListNode_Last(

        struct DoubleLinkedBplusList* LinkedList,
        BplusListData RecordData

)
{


    struct BplusListNode* TempNode = Create_BplusListNode(RecordData);


    if (LinkedList->List_HeadNode == NULL && LinkedList->List_TailNode == NULL)
    {

        LinkedList->List_HeadNode = TempNode;
        LinkedList->List_TailNode = TempNode;

        LinkedList->List_Size++;

        return;

    }


    TempNode->List_PrevNode = LinkedList->List_TailNode;
    LinkedList->List_TailNode->List_NextNode = TempNode;
    LinkedList->List_TailNode = TempNode;

    LinkedList->List_Size++;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void InsertBplusListNode_Index(

        struct DoubleLinkedBplusList* LinkedList,
        BplusListData RecordData,
        int insertion_position_index

)
{


    int middle_position;


    if ((int)(LinkedList->List_Size % 2) == 0)
    {

        middle_position = (LinkedList->List_Size) / 2 - 1;

    }
    else {

        middle_position = (LinkedList->List_Size) / 2;

    }


    if (insertion_position_index == 0)
    {

        InsertBplusListNode_First(LinkedList, RecordData);

        return;

    }


    if (insertion_position_index == LinkedList->List_Size)
    {

        InsertBplusListNode_Last(LinkedList, RecordData);

        return;

    }


    if (insertion_position_index > 0 && insertion_position_index <= middle_position)
    {

        struct BplusListNode* InputNode = Create_BplusListNode(RecordData);
        struct BplusListNode* TempNode = LinkedList->List_HeadNode;
        int i = 0;


        for (i = 0; i < insertion_position_index; i++)
        {

            TempNode = TempNode->List_NextNode;

        }


        TempNode->List_PrevNode->List_NextNode = InputNode;
        InputNode->List_PrevNode = TempNode->List_PrevNode;
        InputNode->List_NextNode = TempNode;
        TempNode->List_PrevNode = InputNode;

        LinkedList->List_Size++;

        return;

    }


    if (insertion_position_index > middle_position && insertion_position_index < LinkedList->List_Size)
    {

        struct BplusListNode* InputNode = Create_BplusListNode(RecordData);
        struct BplusListNode* TempNode = LinkedList->List_TailNode;
        int i = 0, Size = LinkedList->List_Size - 1;


        for (i = 0; i < (Size - insertion_position_index); i++)
        {

            TempNode = TempNode->List_PrevNode;

        }


        TempNode->List_PrevNode->List_NextNode = InputNode;
        InputNode->List_PrevNode = TempNode->List_PrevNode;
        InputNode->List_NextNode = TempNode;
        TempNode->List_PrevNode = InputNode;

        LinkedList->List_Size++;

        return;

    }


    return;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BplusListData DeleteBplusListNode_First(

        struct DoubleLinkedBplusList* LinkedList

)
{


    if (LinkedList->List_HeadNode == NULL && LinkedList->List_TailNode == NULL)
    {

        printf("List is empty\n");

        return NULL;

    }


    BplusListData CopyData;


    if (LinkedList->List_HeadNode == LinkedList->List_TailNode)
    {

        CopyData = LinkedList->List_HeadNode->RecordData;
        free(LinkedList->List_HeadNode);

        Initialize_BplusList(LinkedList);

        return CopyData;

    }


    CopyData = LinkedList->List_HeadNode->RecordData;

    LinkedList->List_HeadNode = LinkedList->List_HeadNode->List_NextNode;
    LinkedList->List_HeadNode->List_PrevNode->List_NextNode = NULL;
    free(LinkedList->List_HeadNode->List_PrevNode);
    LinkedList->List_HeadNode->List_PrevNode = NULL;

    LinkedList->List_Size--;

    return CopyData;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BplusListData DeleteBplusListNode_Last(

        struct DoubleLinkedBplusList* LinkedList

)
{


    if (LinkedList->List_HeadNode == NULL && LinkedList->List_TailNode == NULL)
    {

        printf("List is empty\n");

        return NULL;

    }


    BplusListData CopyData;


    if (LinkedList->List_HeadNode == LinkedList->List_TailNode)
    {

        CopyData = LinkedList->List_TailNode->RecordData;
        free(LinkedList->List_TailNode);

        Initialize_BplusList(LinkedList);

        return CopyData;

    }


    CopyData = LinkedList->List_TailNode->RecordData;

    LinkedList->List_TailNode = LinkedList->List_TailNode->List_PrevNode;
    LinkedList->List_TailNode->List_NextNode->List_PrevNode = NULL;
    free(LinkedList->List_TailNode->List_NextNode);
    LinkedList->List_TailNode->List_NextNode = NULL;

    LinkedList->List_Size--;

    return CopyData;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BplusListData DeleteBplusListNode_Index(

        struct DoubleLinkedBplusList* LinkedList,
        int deletion_position_index

)
{


    int middle_position;


    if ((int)(LinkedList->List_Size % 2) == 0)
    {

        middle_position = (LinkedList->List_Size) / 2 - 1;

    }
    else {

        middle_position = (LinkedList->List_Size) / 2;

    }


    if (deletion_position_index == 0)
    {

        return DeleteBplusListNode_First(LinkedList);

    }


    if (deletion_position_index == LinkedList->List_Size - 1)
    {

        return DeleteBplusListNode_Last(LinkedList);

    }


    if (deletion_position_index > 0 && deletion_position_index <= middle_position)
    {

        struct BplusListNode* TempNode = LinkedList->List_HeadNode;
        int i = 0;
        BplusListData CopyData;


        for (i = 0; i < deletion_position_index; i++)
        {

            TempNode = TempNode->List_NextNode;

        }


        CopyData = TempNode->RecordData;

        TempNode->List_PrevNode->List_NextNode = TempNode->List_NextNode;
        TempNode->List_NextNode->List_PrevNode = TempNode->List_PrevNode;
        TempNode->List_NextNode = NULL;
        TempNode->List_PrevNode = NULL;

        free(TempNode);

        LinkedList->List_Size--;

        return CopyData;

    }


    if (deletion_position_index > middle_position && deletion_position_index < LinkedList->List_Size)
    {

        struct BplusListNode* TempNode = LinkedList->List_TailNode;
        int i = 0, Size = LinkedList->List_Size - 1;
        BplusListData CopyData;


        for (i = 0; i < (Size - deletion_position_index); i++)
        {

            TempNode = TempNode->List_PrevNode;

        }


        CopyData = TempNode->RecordData;

        TempNode->List_PrevNode->List_NextNode = TempNode->List_NextNode;
        TempNode->List_NextNode->List_PrevNode = TempNode->List_PrevNode;
        TempNode->List_NextNode = NULL;
        TempNode->List_PrevNode = NULL;

        LinkedList->List_Size--;

        return CopyData;

    }


    return NULL;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void DeleteBplusList(

        struct DoubleLinkedBplusList* LinkedList

)
{


    if (LinkedList->List_HeadNode == NULL && LinkedList->List_TailNode == NULL)
    {

        printf("List is empty\n");

        return;

    }


    BplusListNode* TempNode = LinkedList->List_HeadNode;
    BplusListNode* DeletedNode;


    while (TempNode != NULL)
    {

        DeletedNode = TempNode;
        TempNode = TempNode->List_NextNode;

        free(DeletedNode);

    }


    Initialize_BplusList(LinkedList);


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void DropBplusList(

        struct DoubleLinkedBplusList* LinkedList,

        bool (*Deallocate_Record_Data)(BplusListData)

)
{


    if (LinkedList->List_HeadNode == NULL && LinkedList->List_TailNode == NULL)
    {

        printf("List is empty\n");

        return;

    }


    BplusListNode* TempNode = LinkedList->List_HeadNode;
    BplusListNode* DeletedNode;


    while (TempNode != NULL)
    {

        DeletedNode = TempNode;
        TempNode = TempNode->List_NextNode;

        Deallocate_Record_Data(DeletedNode->RecordData);

        free(DeletedNode);

    }


    Initialize_BplusList(LinkedList);


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void DisplayBplusList_Forward(

        struct DoubleLinkedBplusList LinkedList,

        BplusTreePrimaryKey (*Get_Record_PrimaryKey)(BplusListData)

)
{


    if (LinkedList.List_HeadNode == NULL && LinkedList.List_TailNode == NULL)
    {

        printf("List is empty\n");

        return;

    }


    printf("List : { ");


    while (LinkedList.List_HeadNode != NULL)
    {

        printf("%d ", Get_Record_PrimaryKey(LinkedList.List_HeadNode->RecordData));
        LinkedList.List_HeadNode = LinkedList.List_HeadNode->List_NextNode;

    }


    printf("}\n");


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void DisplayBplusList_Backward(

        struct DoubleLinkedBplusList LinkedList,

        void* Get_RecordPrimaryKey

)
{


    if (LinkedList.List_HeadNode == NULL && LinkedList.List_TailNode == NULL)
    {

        printf("List is empty\n");

        return;

    }


    printf("List : { ");


    while (LinkedList.List_TailNode != NULL)
    {

        printf("%d ", ((BplusTreePrimaryKey (*)(BplusTreeData))(Get_RecordPrimaryKey))(LinkedList.List_TailNode->RecordData));
        LinkedList.List_TailNode = LinkedList.List_TailNode->List_PrevNode;

    }


    printf("}\n");


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void InsertBplusListPart_First(

        struct DoubleLinkedBplusList* LinkedList,
        struct DoubleLinkedBplusList InputLinkedListPart

)
{


    if (InputLinkedListPart.List_HeadNode == NULL && InputLinkedListPart.List_TailNode == NULL)
    {

        printf("Input List is empty\n");

        return;

    }


    if (LinkedList->List_HeadNode == NULL && LinkedList->List_TailNode == NULL)
    {

        LinkedList->List_HeadNode = InputLinkedListPart.List_HeadNode;
        LinkedList->List_TailNode = InputLinkedListPart.List_TailNode;

        LinkedList->List_Size = InputLinkedListPart.List_Size;

        return;

    }


    InputLinkedListPart.List_TailNode->List_NextNode = LinkedList->List_HeadNode;
    LinkedList->List_HeadNode->List_PrevNode = InputLinkedListPart.List_TailNode;
    LinkedList->List_HeadNode = InputLinkedListPart.List_HeadNode;

    LinkedList->List_Size += InputLinkedListPart.List_Size;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void InsertBplusListPart_Last(

        struct DoubleLinkedBplusList* LinkedList,
        struct DoubleLinkedBplusList InputLinkedListPart

)
{


    if (InputLinkedListPart.List_HeadNode == NULL && InputLinkedListPart.List_TailNode == NULL)
    {

        printf("Input List is empty\n");

        return;

    }


    if (LinkedList->List_HeadNode == NULL && LinkedList->List_TailNode == NULL)
    {

        LinkedList->List_HeadNode = InputLinkedListPart.List_HeadNode;
        LinkedList->List_TailNode = InputLinkedListPart.List_TailNode;

        LinkedList->List_Size = InputLinkedListPart.List_Size;

        return;

    }


    InputLinkedListPart.List_HeadNode->List_PrevNode = LinkedList->List_TailNode;
    LinkedList->List_TailNode->List_NextNode = InputLinkedListPart.List_HeadNode;
    LinkedList->List_TailNode = InputLinkedListPart.List_TailNode;

    LinkedList->List_Size += InputLinkedListPart.List_Size;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void InsertBplusListPart_Index(

        struct DoubleLinkedBplusList* LinkedList,
        struct DoubleLinkedBplusList InputLinkedListPart,
        int insertion_position_index

)
{


    int middle_position;


    if ((int)(LinkedList->List_Size % 2) == 0)
    {

        middle_position = (LinkedList->List_Size) / 2 - 1;

    }
    else {

        middle_position = (LinkedList->List_Size) / 2;

    }


    if (insertion_position_index == 0)
    {

        InsertBplusListPart_First(LinkedList, InputLinkedListPart);

        return;

    }


    if (insertion_position_index == LinkedList->List_Size)
    {

        InsertBplusListPart_Last(LinkedList, InputLinkedListPart);

        return;

    }


    if (insertion_position_index > 0 && insertion_position_index <= middle_position)
    {

        struct BplusListNode* TempNode = LinkedList->List_HeadNode;
        int i = 0;


        for (i = 0; i < insertion_position_index; i++)
        {

            TempNode = TempNode->List_NextNode;

        }


        TempNode->List_PrevNode->List_NextNode = InputLinkedListPart.List_HeadNode;
        InputLinkedListPart.List_HeadNode->List_PrevNode = TempNode->List_PrevNode;
        InputLinkedListPart.List_TailNode->List_NextNode = TempNode;
        TempNode->List_PrevNode = InputLinkedListPart.List_TailNode;

        LinkedList->List_Size += InputLinkedListPart.List_Size;

        return;

    }


    if (insertion_position_index > middle_position && insertion_position_index < LinkedList->List_Size)
    {

        struct BplusListNode* TempNode = LinkedList->List_TailNode;
        int i = 0, Size = LinkedList->List_Size - 1;


        for (i = 0; i < (Size - insertion_position_index); i++)
        {

            TempNode = TempNode->List_PrevNode;

        }


        TempNode->List_PrevNode->List_NextNode = InputLinkedListPart.List_HeadNode;
        InputLinkedListPart.List_HeadNode->List_PrevNode = TempNode->List_PrevNode;
        InputLinkedListPart.List_TailNode->List_NextNode = TempNode;
        TempNode->List_PrevNode = InputLinkedListPart.List_TailNode;

        LinkedList->List_Size += InputLinkedListPart.List_Size;

        return;

    }


    return;


}

