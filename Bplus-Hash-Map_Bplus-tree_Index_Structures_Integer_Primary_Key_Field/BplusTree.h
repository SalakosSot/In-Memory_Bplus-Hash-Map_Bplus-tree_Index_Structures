
#include "DoubleLinkedBplusList.h"


typedef struct BplusTree BplusTree;
typedef struct BplusTreeNode* BplusTreeNode;
typedef struct BplusTreeRecordData* BplusTreeData;

typedef struct TableC_RecordData* TableC_RecordData;

typedef int BplusTreePrimaryKey;



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/* General record data - row of a RDBMS table that is linked to the Bplus-tree index structure. */

struct BplusTreeRecordData
{

    void* RecordData;   // Record data - row of RDBMS a table.
    int table_id;       // Table id identifier. Identifies that the record data corresponds and is stored to a specific RDBMS table.

};


/* Record data - row of the table TableC that is linked to the Bplus-tree index structure. */

struct TableC_RecordData
{

    BplusTreePrimaryKey Record_PrimaryKey;   // Primary key field of the record.
    int Field_Data;                          // A not specific - general field of the record.

};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*  Bplus-tree node - basic structural part of the Bplus-tree index. */

struct BplusTreeNode
{

    BplusTreeData* RecordDataArray;   // Dynamic array structure that stores the record references of a table.
    BplusTreeNode* NodeLinkArray;     // Dynamic array structure that stores the next structure level node references of the Bplus-tree index structure.

    bool leaf_type;                   // Flag to specify the node type (internal - leaf node):
                                      //
                                      // Leaf node     - (value: 1)
                                      // Internal node - (value: 0)

    BplusTreeNode NextLeafNode;       // Stores the next right Bplus-tree node.
    BplusTreeNode PreviousLeafNode;   // Stores the previous left Bplus-tree node.
    int array_stored_elements;        // Record references that are stored in the node array structure.
    int array_available_capacity;     // Available storage capacity (allocated memory cells) of the node array structure.

};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/* Bplus-tree index structure. */

struct BplusTree
{

    BplusTreeNode Root;   // Root node of the Bplus-tree index structure.
    int node_size;        // Maximum record references that can be stored in the node array structure.
    int size;             // Stored records in the Bplus-tree index structure.
    int height;           // Number of nodes levels of the Bplus-tree index structure.

};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Initializes the Bplus-tree index structure.
*
*  Return value:
*
*  value: 0 - initialization is successful
*  value: 1 - initialization is unsuccessful
*
*/

bool InitializeBplustree(

        BplusTree* BplusTree,   // Bplus-tree index structure object.
        int node_size           // Maximum record references that can be stored in the node array structure.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Inserts a record data in the Bplus-tree index structure that is linked to a table based on a primary key field.
*
*  Return value:
*
*  value: 0 - insertion is successful
*  value: 1 - insertion is unsuccessful
*
*/

bool BplusTreeInsertData(

        BplusTree* BplusTree,                                           // Bplus-tree index structure object.
        BplusTreeData RecordData,                                       // Record data to be inserted in the Bplus-tree.

        BplusTreePrimaryKey (*Get_Record_PrimaryKey)(BplusTreeData),   // Receives the primary key field of the table that is linked to the Bplus-tree.
        bool (*Deallocate_Record_Data)(BplusTreeData)                  // Deallocates the record data of the table that is linked to the Bplus-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Inserts a record data and rebuilds nodes in the Bplus-tree index structure based on a primary key field.
*  Especially implements the root node split, re-balancing and reconstruction processes.
*
*
*     (Root node is a leaf node and is full)
*
*         [ E1 ] [ E2 ]  <--[ E3 ]                        [ E2 ]
*        /      |      \             --->                /      \
*      NULL    NULL   NULL                  [ E1 ] [ E2 ]        [ E3 ]
*
*
*     (Root node is an internal node and is full)
*
*                 [ E2 ]   [ E4 ]                                                             [ E4 ]
*              /         |         \                           --->                   /                    \
*   [ E1 ] [ E2 ]  [ E3 ] [ E4 ]   [ E5 ] [ E6 ]  <--[ E7 ]                    [ E2 ]                        [ E6 ]
*                                                                             /       \                     /      \
*                                                                    [ E1 ][ E2 ]   [ E3 ][ E4 ] [ E5 ][ E6 ]       [ E7 ]
*
*
*  Return value:
*
*  value: 0 - insertion is successful
*  value: 1 - insertion is unsuccessful
*
*/

bool BplusTreeInsertNode_RootBreakTool(

        BplusTree* BplusTree,                                          // Bplus-tree index structure object.
        BplusTreeData RecordData,                                      // Record data to be inserted in the Bplus-tree.
        int node_size,                                                 // Maximum record references that can be stored in the node array structure.

        BplusTreePrimaryKey (*Get_Record_PrimaryKey)(BplusTreeData),   // Receives the primary key field of the table that is linked to the Bplus-tree.
        bool (*Deallocate_Record_Data)(BplusTreeData)                  // Deallocates the record data of the table that is linked to the Bplus-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Inserts a record data and rebuilds nodes in the Bplus-tree index structure based on a primary key field.
*  Especially implements the leaf and internal levels nodes split, re-balancing and reconstruction processes.
*
*
*     1) The record to be inserted can be stored in the leaf node
*
*                             [ E4 ]                                                                 [ E4 ]
*                     /                    \                                                 /                    \
*               [ E2 ]                      [ E6 ]                   --->              [ E2 ]                      [ E6 ]
*              /      \                    /      \                                   /      \                    /      \
*     [ E1 ][ E2 ]  [ E3 ][ E4 ]  [ E5 ][ E6 ]  [ E7 ]  <--[ E8 ]            [ E1 ][ E2 ]  [ E3 ][ E4 ]  [ E5 ][ E6 ]  [ E7 ][ E8 ]
*
*
*     2) The record can not be stored in the leaf node and the upper level linked node has available capacity
*
*                             [ E4 ]                                                                      [ E4 ]
*                     /                    \                                                      /                    \
*               [ E2 ]                      [ E6 ]                        --->              [ E2 ]                      [ E6 ]     [ E8 ]
*              /      \                    /      \                                        /      \                    /        |        \
*     [ E1 ][ E2 ]  [ E3 ][ E4 ]  [ E5 ][ E6 ]  [ E7 ][ E8 ]  <--[ E9 ]           [ E1 ][ E2 ]  [ E3 ][ E4 ]  [ E5 ][ E6 ] [ E7 ][ E8 ]   [ E9 ]
*
*
*     3) The record can not be stored in the leaf node and the upper level linked node has not available capacity
*
*                             [ E4 ]                                                                                       [ E4 ]                   [ E8 ]
*                     /                    \                                                                       /                       |                     \
*               [ E2 ]                      [ E6 ]     [ E8 ]                             --->               [ E2 ]                      [ E6 ]                   [ E10 ]
*              /      \                    /        |        \                                              /      \                    /      \                 /       \
*     [ E1 ][ E2 ]  [ E3 ][ E4 ]  [ E5 ][ E6 ] [ E7 ][ E8 ]   [ E9 ][ E10 ]  <--[ E11 ]            [ E1 ][ E2 ]  [ E3 ][ E4 ]  [ E5 ][ E6 ]  [ E7 ][ E8 ]  [ E9 ][ E10 ]  [ E11 ]
*
*
*  Return value:  Bplus-tree node object
*
*/

BplusTreeNode BplusTreeInsertNode_Tool(

        BplusTreeNode CurrentNode,                                     // Bplus-tree node.
        BplusTreeData RecordData,                                      // Record data to be inserted in the Bplus-tree.
        BplusTreeData* Node_MiddleRecordData,                          // Middle record data of a specific node record reference array structure.
        int node_size,                                                 // Maximum record references that can be stored in the node array structure.

        bool* flag_duplicate_record,                                   // Flag to specify if the record (to be inserted) already exists in the Bplus-tree index structure:
                                                                       //
                                                                       // Exists     - (value: 1)
                                                                       // Not Exists - (value: 0)

        BplusTreePrimaryKey (*Get_Record_PrimaryKey)(BplusTreeData),   // Receives the primary key field of the table that is linked to the Bplus-tree.
        bool (*Deallocate_Record_Data)(BplusTreeData)                  // Deallocates the record data of the table that is linked to the Bplus-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Locates the record data (based on a primary key field) in a record array structure or
*  the position in a node array structure (next level node) that the record can be located - found.
*
*  Return value:  Position of the record to be located in the record - node array structures of the Bplus-tree node
*
*/

int SearchBplusTreeNode_Record_ByPrimaryKey(

        int array_stored_elements,                                    // Record references that are stored in the node array structure.
        BplusTreeData* RecordDataArray,                               // Record data array of the Bplus-tree node where the search process is implemented.
        BplusTreePrimaryKey PrimaryKey,                               // Primary key field - to be located.

        BplusTreePrimaryKey (*Get_Record_PrimaryKey)(BplusTreeData)   // Receives the primary key field of the table that is linked to the Bplus-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Binary search of an array structure.
*
*  Return value:  Position of the record to be located in the record - node array structures of the Bplus-tree node
*
*/

int BplusTree_Binary_Search(

        int array_stored_elements,                                    // Record references that are stored in the node array structure.
        BplusTreeData* RecordDataArray,                               // Record data array of the Bplus-tree node where the search process is implemented.
        BplusTreePrimaryKey PrimaryKey,                               // Primary key field - to be located.

        BplusTreePrimaryKey (*Get_Record_PrimaryKey)(BplusTreeData)   // Receives the primary key field of the table that is linked to the Bplus-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Interpolation search of an array structure.
*
*  Return value:  Position of the record to be located in the record - node array structures of the Bplus-tree node
*
*/

int BplusTree_Interpolation_Search(

        int array_stored_elements,                                    // Record references that are stored in the node array structure.
        BplusTreeData* RecordDataArray,                               // Record data array of the Bplus-tree node where the search process is implemented.
        BplusTreePrimaryKey PrimaryKey,                               // Primary key field - to be located.

        BplusTreePrimaryKey (*Get_Record_PrimaryKey)(BplusTreeData)   // Receives the primary key field of the table that is linked to the Bplus-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Displays in order the primary key field of every Bplus-tree node record data.
*
*  Return value:  Nothing specific
*
*/

void BplusTreeDisplayInOrder(

        BplusTree BplusTree,                                          // Bplus-tree index structure object.

        BplusTreePrimaryKey (*Get_Record_PrimaryKey)(BplusTreeData)   // Receives the primary key field of the table that is linked to the Bplus-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Displays in order the primary key field of every Bplus-tree node record data.
*  Implements the basic - main display process.
*
*  Return value:  Nothing specific
*
*/

void BplusTreeInOrderTraversal(

        BplusTreeNode CurrentNode,                                    // Bplus-tree node.

        BplusTreePrimaryKey (*Get_Record_PrimaryKey)(BplusTreeData)   // Receives the primary key field of the table that is linked to the Bplus-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Locates the position in the Bplus-tree index structure node where a specific record is stored based on a primary key field.
*
*  Debugging - testing tool.
*
*  Return value:  Record data - to be located
*
*/

BplusTreeData BplusTreeSearchData(

        BplusTree BplusTree,                                          // Bplus-tree index structure object.
        BplusTreePrimaryKey PrimaryKey,                               // Primary key field - to be located.

        BplusTreePrimaryKey (*Get_Record_PrimaryKey)(BplusTreeData)   // Receives the primary key field of the table that is linked to the Bplus-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Locates the position in the Bplus-tree index structure node where a specific record is stored based on a primary key field.
*  Implements the basic - main location process.
*
*  Debugging - testing tool.
*
*  Return value:  Record data - to be located
*
*/

BplusTreeData BplusTreeSearch_Tool(

        BplusTreeNode CurrentNode,                                    // Bplus-tree node.
        BplusTreePrimaryKey PrimaryKey,                               // Primary key field - to be located.

        BplusTreePrimaryKey (*Get_Record_PrimaryKey)(BplusTreeData)   // Receives the primary key field of the table that is linked to the Bplus-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Locates the position in the Bplus-tree index structure node where a specific record is stored based on a primary key field.
*
*  Return value:  Record data - to be located
*
*/

BplusTreeData BplusTreeFastSearchData_ByPrimaryKey(

        BplusTree BplusTree,                                          // Bplus-tree data structure object.
        BplusTreePrimaryKey PrimaryKey,                               // Primary key field - to be located.

        BplusTreePrimaryKey (*Get_Record_PrimaryKey)(BplusTreeData)   // Receives the primary key field of the table that is linked to the Bplus-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Locates the position in the Bplus-tree index structure node where a specific record is stored based on a primary key field.
*  Implements the basic - main location process.
*
*  Return value:  Record data - to be located
*
*/

BplusTreeData BplusTreeFastSearch_Tool(

        BplusTreeNode CurrentNode,                                    // Bplus-tree node.
        BplusTreePrimaryKey PrimaryKey,                               // Primary key field - to be located.

        BplusTreePrimaryKey (*Get_Record_PrimaryKey)(BplusTreeData)   // Receives the primary key field of the table that is linked to the Bplus-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Initializes and creates the record data of the TableC.
*
*  Return value:  Created record data
*
*/

BplusTreeData InitializeBplusTreeData_TableC(

        BplusTreePrimaryKey Record_PrimaryKey,
        int Field_Data

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Receives the primary key field of the TableC.
*
*  Return value:  Primary key field
*
*/

BplusTreePrimaryKey Get_PrimaryKey_TableC(

        BplusTreeData Input_RecordData

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Receives the generic field data of TableC.
*
*  Return value: Field data
*
*/

int Get_FieldData_TableC(

        BplusTreeData Input_RecordData

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Deallocation - deletion of the record data.
*
*  Return value:
*
*  value: 0 - deletion is successful
*  value: 1 - deletion is unsuccessful
*
*/

bool Deallocate_BplusTreeRecordData_TableC(

        BplusTreeData RecordData_ToBe_Deleted   // Record data - to be deleted.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Drops - removes all the nodes and record data from the Βplus-tree.
*
*  Return value:
*
*  value: 0 - B-tree deletion is successful
*  value: 1 - B-tree deletion is unsuccessful
*
*/

bool BplusTreeDrop(

        BplusTree* BplusTree,                           // Bplus-tree index structure object.

        bool (*Deallocate_Record_Data)(BplusTreeData)   // Deallocates the record data of the table that is linked to the Bplus-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Drops - removes all the nodes and record data from the Βplus-tree.
*  Especially performs the main part of the deletion process.
*
*  Return value:  Nothing specific
*
*/

void BplusTreeDropNode(

        BplusTreeNode CurrentNode,                      // Bplus-tree node.

        bool (*Deallocate_Record_Data)(BplusTreeData)   // Deallocates the record data of the table that is linked to the Bplus-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Selects the record data (ascending - by a non-primary field) in the bottom - leaf level of the Bplus-tree
*  and stores that record data in a Double Linked List structure.
*
*  Return value:
*
*  value: 0 - selection is successful
*  value: 1 - selection is unsuccessful
*
*/

bool BplusTreeSelectRecordData_ASC(

        BplusTree BplusTree,                                         // Bplus-tree index structure object.
        struct DoubleLinkedBplusList* SelectionResultList,           // Result list that contains the selected record data.
        int Field_Data,                                              // Field that the record data selection is based on.

        int (*Get_Record_FieldData)(BplusTreeData)   // Receives the generic field data of the table that is linked to the Bplus-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Locates the head - left node of the Bplus-tree bottom - leaf level nodes set (List of leaf nodes).
*
*  Return value:  Head of the leaf nodes List
*
*/

BplusTreeNode BplusTreeLocateLeftLeafNode(

        BplusTree BplusTree   // Bplus-tree index structure object.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Selects the record data (descending - by a non-primary field) in the bottom - leaf level of the Bplus-tree
*  and stores that record data in a Double Linked List structure.
*
*  Return value:
*
*  value: 0 - selection is successful
*  value: 1 - selection is unsuccessful
*
*/

bool BplusTreeSelectRecordData_DESC(

        BplusTree BplusTree,                                         // Bplus-tree index structure object.
        struct DoubleLinkedBplusList* SelectionResultList,           // Result list that contains the selected record data.
        int Field_Data,                                              // Field that the record data selection is based on.

        int (*Get_Record_FieldData)(BplusTreeData)   // Receives the generic field data of the table that is linked to the Bplus-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Locates the tail - right node of the Bplus-tree bottom - leaf level nodes set (List of leaf nodes).
*
*  Return value:  Tail of the leaf nodes List
*
*/

BplusTreeNode BplusTreeLocateRightLeafNode(

        BplusTree BplusTree   // Bplus-tree index structure object.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Selects recursive the record data (ascending - by non-primary field) in the Bplus-tree
*  and stores that record data in a Double Linked List.
*
*  Debugging - testing tool.
*
*  Return value:
*
*  value: 0 - selection is successful
*  value: 1 - selection is unsuccessful
*
*/

bool BplusTreeSelectRecordData_Rec_ASC(

        BplusTree BplusTree,                                // Bplus-tree index structure object.
        struct DoubleLinkedBplusList* SelectionResultList   // Result list that contains the selected record data.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Selects recursive the record data (ascending - by non-primary field) in the Bplus-tree
*  and stores that record data in a Double Linked List.
*  Especially performs the main part of the deletion process.
*
*  Debugging - testing tool.
*
*  Return value:  Nothing specific
*
*/

void BplusTreeSelectRecordData_Rec_ASC_Tool(

        BplusTreeNode CurrentNode,                          // Bplus-tree node.
        struct DoubleLinkedBplusList* SelectionResultList   // Result list that contains the selected record data.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Computes the number of internal and leaf Bplus-tree nodes and their stored record data.
*
*  Debugging - testing tool.
*
*  Return value:
*
*  value: 0 - computation is successful
*  value: 1 - computation is unsuccessful
*
*/

bool BplusTreeComputeNodes(

        BplusTree BplusTree,           // Bplus-tree index structure object.
        int* internal_nodes,           // Number of internal Bplus-tree nodes.
        int* internal_nodes_records,   // Number of internal Bplus-tree nodes stored record data.
        int* leaf_nodes,               // Number of leaf Bplus-tree nodes.
        int* leaf_nodes_records        // Number of leaf Bplus-tree nodes stored record data

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Computes the number of internal and leaf Bplus-tree nodes and their stored record data.
*  Especially performs the main part of the computation process.
*
*  Debugging - testing tool.
*
*  Return value:  Nothing specific
*
*/

void BplusTreeComputeNodes_Tool(

        BplusTreeNode CurrentNode,     // Bplus-tree node.
        int* internal_nodes,           // Number of internal Bplus-tree nodes.
        int* internal_nodes_records,   // Number of internal Bplus-tree nodes stored record data.
        int* leaf_nodes,               // Number of leaf Bplus-tree nodes.
        int* leaf_nodes_records        // Number of leaf Bplus-tree nodes stored record data.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Computes the Bplus-tree structure height.
*
*  Debugging - testing tool.
*
*  Return value:
*
*  value: 0 - computation is successful
*  value: 1 - computation is unsuccessful
*
*/

bool BplusTreeComputeHeight(

        BplusTree BplusTree,   // Bplus-tree index structure object.
        int* height            // Bplus-tree height.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Checks for invalid Bplus-tree nodes record - node array structures capacity and stored data.
*
*  Debugging - testing tool.
*
*  Return value:
*
*  value: 0 - procedure is successful
*  value: 1 - procedure is unsuccessful
*
*/

bool BplusTreeCheckNodeCapacity(

        BplusTree BplusTree,   // Bplus-tree index structure object.

        bool* error_flag       // Flag to check if the Bplus-tree nodes capacity is properly defined:
                               //
                               // Incorrect definition - (value: 1)
                               // Correct definition   - (value: 0)

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Checks for invalid Bplus-tree nodes record - node array structures capacity and stored data.
*  Especially performs the main part of the procedure.
*
*  Debugging - testing tool.
*
*  Return value:  Nothing specific
*
*/

void BplusTreeCheckNodeCapacity_Tool(

        BplusTreeNode CurrentNode,   // Bplus-tree node.
        int node_size,               // Maximum record references that can be stored in the node array structure.

        bool* error_flag             // Flag to check if the Bplus-tree nodes capacity is correctly defined:
                                     //
                                     // Incorrect definition - (value: 1)
                                     // Correct definition   - (value: 0)

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Checks if the Bplus-tree structure is balanced.
*
*  Debugging - testing tool.
*
*  Return value:
*
*  value: 0 - procedure is successful
*  value: 1 - procedure is unsuccessful
*
*/

bool BplusTreeCheckBalance(

        BplusTree BplusTree,    // Bplus-tree index structure object.
        int Bplustree_height,   // Bplus-tree height.

        int* unbalanced_nodes   // Number of unbalanced leaf nodes.
                                // Exception - (value > 0)

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Checks if the Bplus-tree structure is balanced.
*  Especially performs the main part of the procedure.
*
*  Debugging - testing tool.
*
*  Return value:  Nothing specific
*
*/

void BplusTreeCheckBalance_Tool(

        BplusTreeNode CurrentNode,   // Bplus-tree node.
        int node_height,             // Current node height.
        int BplusTree_height,        // Bplus-tree height.

        int* unbalanced_nodes        // Number of unbalanced leaf nodes.
                                     // Exception - (value > 0)

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Checks if the nodes of the bottom - leaf level contain node references array structures.
*
*  Debugging - testing tool.
*
*  Return value:
*
*  value: 0 - procedure is successful
*  value: 1 - procedure is unsuccessful
*
*/

bool BplusTreeCheckLeafLinkNodes(

        BplusTree BplusTree,               // Bplus-tree index structure object.

        int* improperly_structured_nodes   // Number of improperly structured leaf nodes.
                                           // Exception - (value > 0)

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Checks if the nodes of the bottom - leaf level contain node references array structures.
*  Especially performs the main part of the procedure.
*
*  Debugging - testing tool.
*
*  Return value:  Nothing specific
*
*/

void BplusTreeCheckLeafLinkNodes_Tool(

        BplusTreeNode CurrentNode,         // Bplus-tree node.

        int* improperly_structured_nodes   // Number of improperly structured leaf nodes.
                                           // Exception - (value > 0)

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Searches for the location on a Bplus-tree node and the type of the node (internal or leaf node)
*  where there is a record data by a primary key field.
*
*  Debugging - testing tool.
*
*  Return value:  Located record data
*
*/

BplusTreeData BplusTreeSearch_NodeType_ByRecord(

        BplusTree BplusTree,                                          // Bplus-tree index structure object.
        BplusTreePrimaryKey Record_PrimaryKey,                        // Primary key field - to be located.

        int* record_node_type_flag,                                   // Flag to specify the record data type by Bplus-tree node type:
                                                                      //
                                                                      // Internal record type (the record exists in both internal and leaf nodes) - (value: 2)
                                                                      // Leaf record type (the record exists in a leaf node)                      - (value: 1)
                                                                      // Record data not found in any node                                        - (value: 0)

        BplusTreePrimaryKey (*Get_Record_PrimaryKey)(BplusTreeData)   // Receives the primary key field of the table that is linked to the Bplus-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Searches for the location on a Bplus-tree node and the type of the node (internal or leaf node)
*  where there is a record data by a primary key field.
*  Especially performs the main part of the work for the procedure.
*
*  Debugging - testing tool.
*
*  Return value:  Located record data
*
*/

BplusTreeData BplusTreeSearch_NodeType_ByRecord_Tool(

        BplusTreeNode CurrentNode,                                    // Bplus-tree node.
        BplusTreePrimaryKey Record_PrimaryKey,                        // Primary key field - to be located.

        int* record_node_type_flag,                                   // Flag to specify the record data type by Bplus-tree node type:
                                                                      //
                                                                      // Internal record type (the record exists in both internal and leaf nodes) - (value: 2)
                                                                      // Leaf record type (the record exists in a leaf node)                      - (value: 1)
                                                                      // Record data not found in any node                                        - (value: 0)

        BplusTreePrimaryKey (*Get_Record_PrimaryKey)(BplusTreeData)   // Receives the primary key field of the table that is linked to the Bplus-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Checks if all the internal nodes are properly linked (List links of the next and previous node are null).
*
*  Debugging - testing tool.
*
*  Return value:
*
*  value: 0 - procedure is successful
*  value: 1 - procedure is unsuccessful
*
*/

bool BplusTreeCheckInternalNode_ListLinks(

        BplusTree BplusTree,               // Bplus-tree index structure object.

        int* improperly_structured_nodes   // Internal nodes with not null list links.
                                           // Exception - (value > 0)

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Checks if all the internal nodes are properly linked (List links of the next and previous node are null).
*  Especially performs the main part of the work for the procedure.
*
*  Debugging - testing tool.
*
*  Return value:  Nothing specific
*
*/

void BplusTreeCheckInternalNode_ListLinks_Tool(

        BplusTreeNode CurrentNode,         // Bplus-tree node.
        int* improperly_structured_nodes   // Internal nodes with not null list links.
                                           // Exception - (value > 0)

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Deletes a record data that is stored in the Bplus-tree index structure based on a primary key field.
*
*  Return value:
*
*  value: 0 - deletion is successful
*  value: 1 - deletion is unsuccessful
*
*/

bool BplusTreeDeleteData(

        BplusTree* BplusTree,                                         // Bplus-tree index structure object.
        BplusTreePrimaryKey PrimaryKey,                               // Primary key field of the record data to be deleted.
        BplusTreeData* DeletedRecordData,                             // Record data - to be deleted.

        BplusTreePrimaryKey (*Get_Record_PrimaryKey)(BplusTreeData)   // Receives the primary key field of the table that is linked to the Bplus-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Deletes a record data that is stored in the Bplus-tree index structure based on a primary key field.
*  Especially implements the record data deletion in a leaf - internal node and the nodes re-balancing - reconstruction.
*
*  Return value:  B-tree node object
*
*/

BplusTreeNode BplusTreeDeleteNode(

        BplusTreeNode CurrentNode,                                    // Bplus-tree node.
        BplusTreePrimaryKey PrimaryKey,                               // Primary key field of the record data to be deleted.
        BplusTreeData* DeletedRecordData,                             // Record data - to be deleted.
        BplusTreeData* internal_record_node_position,                 // Record data of an internal node array structure that the record to be deleted is stored.
        int node_size,                                                // Maximum record references that can be stored in the node array structure.

        bool* balancing_flag,                                         // Flag to specify if the balancing process has been activated to the non leaf node levels:
                                                                      //
                                                                      // Balance can not be restored to the leaf node level.
                                                                      // Balancing process activation   - (value: 1)
                                                                      // Balancing process inactivation - (value: 0)

        bool* internal_record_deletion_flag,                          // Flag to specify if the internal node record data deletion procedure has been activated:
                                                                      //
                                                                      // The record data to be deleted is in an internal node.
                                                                      // Internal node deletion procedure activation   - (value: 1)
                                                                      // Internal node deletion procedure inactivation - (value: 0)

        bool* record_existence_flag,                                  // Flag to specify if the record data to be deleted exists:
                                                                      //
                                                                      // Record data not exists - (value: 1)
                                                                      // Record data exists     - (value: 0)

        BplusTreePrimaryKey (*Get_Record_PrimaryKey)(BplusTreeData)   // Receives the primary key field of the table that is linked to the Bplus-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Deletes a record data that is stored in an internal node based on a primary key field.
*  Especially implements the internal nodes re-balancing - reconstruction of the upper - internal nodes levels.
*
*  Return value:  Nothing specific
*
*/

void BplusTreeDelete_NonLeafNode(

        BplusTreeNode CurrentNode,   // Bplus-tree node.
        int record_position,         // Position of the next level linked node in the node array structure
                                     // that the nodes set reconstruction - re-balancing process was implemented.

        int node_size,               // Maximum record references that can be stored in the node array structure.

        bool* balancing_flag         // Flag to specify if the balancing process has been activated to the non leaf node levels.
                                     //
                                     // Balance can not be restored to the leaf node level.
                                     // Balancing process activation   - (value: 1)
                                     // Balancing process inactivation - (value: 0)

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Deletes a record data that is stored in a leaf node based on a primary key field.
*  Especially implements the record data deletion in the leaf nodes bottom level
*  and the re-balancing - reconstruction of the nodes set that the deletion was implemented.
*
*  Return value:  Nothing specific
*
*/

void BplusTreeDelete_LeafNode(

        BplusTreeNode CurrentNode,                                    // Bplus-tree node.
        BplusTreePrimaryKey PrimaryKey,                               // Primary key field of the record data to be deleted.
        BplusTreeData* DeletedRecordData,                             // Record data - to be deleted.
        int record_position,                                          // Position of the record data to be deleted in the next level leaf node record array structure.
        BplusTreeData* internal_record_node_position,                 // Record data of an internal node array structure that the record to be deleted is stored.
        int node_size,                                                // Maximum record references that can be stored in the node array structure.

        bool* balancing_flag,                                         // Flag to specify if the balancing process has been activated to the non leaf node levels:
                                                                      //
                                                                      // Balance can not be restored to the leaf node level.
                                                                      // Balancing process activation   - (value: 1)
                                                                      // Balancing process inactivation - (value: 0)

        bool internal_record_deletion_flag,                           // Flag to specify if the internal node record data deletion procedure has been activated:
                                                                      //
                                                                      // The record data to be deleted is in an internal node.
                                                                      // Internal node deletion procedure activation   - (value: 1)
                                                                      // Internal node deletion procedure inactivation - (value: 0)

        bool* record_existence_flag,                                  // Flag to specify if the record data to be deleted exists:
                                                                      //
                                                                      // Record data not exists - (value: 1)
                                                                      // Record data exists     - (value: 0)

        BplusTreePrimaryKey (*Get_Record_PrimaryKey)(BplusTreeData)   // Receives the primary key field of the table that is linked to the Bplus-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Deletes a record data from a leaf node in the case that the leaf node contains multiple records.
*  Implements the deletion nodes set reconstruction - re-balancing.
*
*  Return value:  Nothing specific
*
*/

void BplusTree_ReplaceRecord(

        BplusTreeNode CurrentNode,
        int record_position,
        int node_size

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Deletes a record data from a leaf node in the case that the leaf node contains a single record,
*  the upper level linked node contains a single record and the left side node contains a single record.
*  Implements the deletion nodes set reconstruction - re-balancing.
*
*
*	[ E1 ]                [ E1 ]
*      /      \     ---->    /      \
*  [ E1 ]    [ X ]         Null     Null
*
*
*  Return value:  Nothing specific
*
*/

void BplusTree_MergeLeftNode(

        BplusTreeNode CurrentNode

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Deletes a record data from a leaf node in the case that the leaf node contains a single record,
*  the upper level linked node contains a single record and the right side node contains a single record.
*  Implements the deletion nodes set reconstruction - re-balancing.
*
*
*      [ E1 ]                [ E2 ]
*     /      \     ---->    /      \
*  [ X ]   [ E2 ]         Null    Null
*
*
*  Return value:  Nothing specific
*
*/

void BplusTree_MergeRightNode(

        BplusTreeNode CurrentNode

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Deletes a record data from a leaf node in the case that the leaf node contains a single record,
*  the upper level linked node contains a single record and the left side node contains multiple records.
*  Implements the deletion nodes set reconstruction - re-balancing.
*
*
*	      [ E2 ]                  [ E1 ]
*            /      \     ---->      /      \
*   [ E1 ][ E2 ]   [ X ]         [ E1 ]   [ E2 ]
*
*
*  Return value:  Nothing specific
*
*/

void BplusTree_RebalanceLeftNode(

        BplusTreeNode CurrentNode,
        BplusTreeNode NextLevelNode,
        int node_size

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Deletes a record data from a leaf node in the case that the leaf node contains a single record,
*  the upper level linked node contains a single record and the right side node contains multiple records.
*  Implements the deletion nodes set reconstruction - re-balancing.
*
*
*      [ E1 ]                       [ E2 ]
*     /      \           ---->     /      \
*  [ X ]   [ E2 ][ E3 ]         [ E2 ]   [ E3 ]
*
*
*  Return value:  Nothing specific
*
*/

void BplusTree_RebalanceRightNode(

        BplusTreeNode CurrentNode,
        BplusTreeNode NextLevelNode,
        int node_size

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Deletes a record data from a leaf node in the case that the leaf node contains a single record,
*  the upper level linked node contains multiple records and the right side node contains multiple records.
*  Implements the deletion nodes set reconstruction - re-balancing.
*
*
*	         [ E1 ]   [ E3 ]                               [ E3 ]
*              /        |        \       --->                /        \
*         [ X ]  [  E2 ] [ E3 ]  [ E4 ]         [ E2 ] [ E3 ]          [ E4 ]
*
*
*  Return value:  Nothing specific
*
*/

void BplusTree_ReplaceRecord_Right_to_Left(

        BplusTreeNode CurrentNode,
        BplusTreeNode NextLevelNode,
        int record_position,
        int node_size

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Deletes a record data from a leaf node in the case that the leaf node contains a single record,
*  the upper level linked node contains multiple records and the left side node contains multiple records.
*  Implements the deletion nodes set reconstruction - re-balancing.
*
*
*	             [ E1 ]   [ E3 ]                       [ E1 ]
*                  /        |        \       --->         /      \
*            [ E1 ]   [ E2 ] [ E3 ]   [ X ]         [ E1 ]        [ E2 ] [ E3 ]
*
*
*  Return value:  Nothing specific
*
*/

void BplusTree_ReplaceRecord_Left_to_Right(

        BplusTreeNode CurrentNode,
        BplusTreeNode NextLevelNode,
        int record_position,
        int node_size

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Implements the internal nodes set reconstruction - re-balancing in the case that
*  the next (under) level linked node reconstruction - re-balancing was implemented unsuccessfully.
*
*  The upper level linked node contains a single record and the left side node contains a single record.
*
*
*               [ E2 ]                      [ E1 ] [ E2 ]
*              /      \                    /      |      \
*          [ E1 ]      [ E3 ]  --->  [ E1 ]    [ E2 ]     [ E3 ]
*         /      \
*     [ E1 ]      [ E2 ]
*
*
*  Return value:  Nothing specific
*
*/

void BplusTree_MergeLeftNodeRecursive(

        BplusTreeNode CurrentNode,
        BplusTreeNode LeftNode,
        int node_size

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Implements the internal nodes set reconstruction - re-balancing in the case that
*  the next (under) level linked node reconstruction - re-balancing was implemented unsuccessfully.
*
*  The upper level linked node contains a single record and the left side node contains multiple records.
*
*
*                          [ E3 ]                                 [ E2 ]
*                         /      \                            /            \
*            [ E1 ] [ E2 ]        [ E4 ]  --->         [ E1 ]               [ E3 ]
*           /      |      \                          /        \            /      \
*     [ E1 ]     [ E2 ]    [ E3 ]              [ E1 ]          [ E2 ]   [ E3]    [ E4 ]
*
*
*  Return value:  Nothing specific
*
*/

void BplusTree_ReplaceLeftNodeRecursive(

        BplusTreeNode CurrentNode,
        BplusTreeNode LeftNode,
        int node_size

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Implements the internal nodes set reconstruction - re-balancing in the case that
*  the next (under) level linked node reconstruction - re-balancing was implemented unsuccessfully.
*
*  The upper level linked node contains a single record and the right side node contains a single record.
*
*  Return value:  Nothing specific
*
*/

void BplusTree_MergeRightNodeRecursive(

        BplusTreeNode CurrentNode,
        BplusTreeNode RightNode,
        int node_size

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Implements the internal nodes set reconstruction - re-balancing in the case that
*  the next (under) level linked node reconstruction - re-balancing was implemented unsuccessfully.
*
*  The upper level linked node contains a single record and the right side node contains multiple records.
*
*  Return value:  Nothing specific
*
*/

void BplusTree_ReplaceRightNodeRecursive(

        BplusTreeNode CurrentNode,
        BplusTreeNode RightNode,
        int node_size

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Implements the internal nodes set reconstruction - re-balancing in the case that
*  the next (under) level linked node reconstruction - re-balancing was implemented unsuccessfully.
*
*  The upper level linked node contains multiple records and the left side node contains multiple records.
*
*
*                          [ E3 ]   [ E5 ]                                       [ E2 ]     [ E5 ]
*                       /         |        \                                  /          |          \
*          [ E1 ] [ E2 ]       [ E5 ]   [ E6 ]        --->             [ E1 ]          [ E3 ]         [ E6 ]
*         /      |     \               /      \                       /      \        /      \       /      \
*     [ E1 ]  [ E2 ]  [ E3 ]       [ E6 ]   [ E7 ]                [ E1 ]  [ E2 ] [ E3 ]     [ E5 ] [ E6 ]   [ E7 ]
*                                                                                          
*
*  Return value:  Nothing specific
*
*/


void BplusTree_ReplaceMultipleLeftNodeRecursive(

        BplusTreeNode CurrentNode,
        BplusTreeNode LeftNode,
        int record_position,
        int node_size

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Implements the internal nodes set reconstruction - re-balancing in the case that
*  the next (under) level linked node reconstruction - re-balancing was implemented unsuccessfully.
*
*  The upper level linked node contains multiple records and the left side node contains a single record.
*
*
*                   [ E2 ]   [ E4 ]                                             [ E4 ]
*                 /        |        \                                      /              \
*          [ E1 ]       [ E4 ]       [ E5 ]        --->       [ E1 ] [ E2 ]                [ E5 ]
*         /      \                  /      \                 /      |      \              /      \
*      [ E1 ]  [ E2 ]            [ E5 ]  [ E6 ]           [ E1 ] [ E2 ]  [ E4 ]        [ E5 ]  [ E6 ]
*
*
*  Return value:  Nothing specific
*
*/

void BplusTree_ReplaceSingleLeftNodeRecursive(

        BplusTreeNode CurrentNode,
        BplusTreeNode LeftNode,
        int record_position,
        int node_size

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Implements the internal nodes set reconstruction - re-balancing in the case that
*  the next (under) level linked node reconstruction - re-balancing was implemented unsuccessfully.
*
*  The upper level linked node contains multiple records and the right side node contains multiple records.
*
*  Return value:  Nothing specific
*
*/

void BplusTree_ReplaceMultipleRightNodeRecursive(

        BplusTreeNode CurrentNode,
        BplusTreeNode RightNode,
        int record_position,
        int node_size

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Implements the internal nodes set reconstruction - re-balancing in the case that
*  the next (under) level linked node reconstruction - re-balancing was implemented unsuccessfully.
*
*  The upper level linked node contains multiple records and the right side node contains a single record.
*
*  Return value:  Nothing specific
*
*/

void BplusTree_ReplaceSingleRightNodeRecursive(

        BplusTreeNode CurrentNode,
        BplusTreeNode RightNode,
        int record_position,
        int node_size

);

