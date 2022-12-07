
#include "BplusTree.h"


typedef struct BplusHashMap BplusHashMap;
typedef struct BplusTreeRecordData* BplusHashMapData;



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/* BplusHashMap index structure. */

struct BplusHashMap
{

    BplusTree** BplusTreeArray;    // Array structure of Bplus-tree index structures.
    int BplusHashMap_Level_Size;   // Set of BplusHashMap Bplus-tree index structures.
    int size;                      // Stored records in the BplusHashMap Bplus-tree index structures.

};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Initializes the BplusHashMap index structure.
*
*  Return value:  Nothing specific
*
*/

void InitializeBplusHashMap(

        BplusHashMap* BplusHashMap,    // BplusHashMap index structure object.
        int BplusHashMap_Level_Size,   // Set of BplusHashMap Bplus-tree index structures.
        int BplusTreeNodeSize          // Maximum record references that can be stored in a Bplus-tree structure node of the BplusHashMap.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Inserts record data in a Bplus-tree structure of the BplusHashMap
*  based on a hash code value of a specific primary key field.
*
*  Return value:
*
*  value: 0 - insertion is successful
*  value: 1 - insertion is unsuccessful
*
*/

bool BplusHashMapInsertData(

        BplusHashMap* BplusHashMap,                                    // BplusHashMap index structure object.
        BplusHashMapData RecordData,                                   // Record data - to be inserted.
        int hash_code,                                                 // Hash code value to locate the insertion position - Bplus-tree index structure
                                                                       // that the record will be stored.

        BplusTreePrimaryKey (*Get_Record_PrimaryKey)(BplusTreeData),   // Receives the primary key field of the table that is linked to the BplusHashMap Bplus-tree.
        bool (*Deallocate_Record_Data)(BplusTreeData)                  // Deallocates the record data of the table that is linked to the BplusHashMap Bplus-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Deletes record data in a Bplus-tree structure of the BplusHashMap
*  based on a hash code value and a primary key field of a specific primary key field.
*
*  Return value:
*
*  value: 0 - deletion is successful
*  value: 1 - deletion is unsuccessful
*
*/

bool BplusHashMapDeleteData(

        BplusHashMap* BplusHashMap,                                   // BplusHashMap index structure object.
        BplusTreePrimaryKey PrimaryKey,                               // Primary key field of the record - to be deleted.
        int hash_code,                                                // Hash code value to locate the deletion position - Bplus-tree index structure
                                                                      // from which the record will be removed.
        BplusHashMapData* DeletedRecordData,                          // Record data - to be deleted.

        BplusTreePrimaryKey (*Get_Record_PrimaryKey)(BplusTreeData)   // Receives the primary key field of the table that is linked to the BplusHashMap Bplus-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Displays the BplusHashMap record data by a primary key fields.
*
*  Return value:  Nothing specific
*
*/

void BplusHashMapDisplay(

        BplusHashMap BplusHashMap,                                    // BplusHashMap index structure object.

        BplusTreePrimaryKey (*Get_Record_PrimaryKey)(BplusTreeData)   // Receives the primary key field of the table that is linked to the BplusHashMap Bplus-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Selects record data from a Bplus-tree structure of the BplusHashMap based on a non-primary field.
*
*  Return value:
*
*  value: 0 - selection is successful
*  value: 1 - selection is unsuccessful
*
*/

bool BplusHashMapSelectData(

        BplusHashMap BplusHashMap,                           // BplusHashMap index structure object.
        struct DoubleLinkedBplusList* SelectionResultList,   // Result list that contains the selected record data.
        int Field_Data,                                      // Field that the record data selection is based on.

        bool selection_method_flag,                          //Flag to set the selection method (ASC - ascending and DESC - descending)
                                                             // of the BplusHashMap Bplus-tree structures parts:
                                                             //
                                                             // ASC selection  - (value: 0)
                                                             // DESC selection - (value: 1)

        int (*Get_Record_FieldData)(BplusTreeData)           // Receives the generic field data of the table that is linked to the BplusHashMap Bplus-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Selects record data from a Bplus-tree structure of the BplusHashMap based on a primary key field.
*
*  Return value:  Selected record data
*
*/

BplusHashMapData BplusHashMapSelectData_ByPrimaryKey(

        BplusHashMap BplusHashMap,                                    // BplusHashMap index structure object.
        BplusTreePrimaryKey PrimaryKey,                               // Primary key field of the record - to be selected.
        int hash_code,                                                // Hash code value to locate the position - Bplus-tree index structure
                                                                      // in witch the record is stored.

        BplusTreePrimaryKey (*Get_Record_PrimaryKey)(BplusTreeData)   // Receives the primary key field of the table that is linked to the BplusHashMap Bplus-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Drops completely the BplusHashMap structure and the stored records set.
*
*  Return value:
*
*  value: 0 - deletion is successful
*  value: 1 - deletion is unsuccessful
*
*/

bool BplusHashMapDrop(

        BplusHashMap* BplusHashMap,                     // BplusHashMap index structure object.

        bool (*Deallocate_Record_Data)(BplusTreeData)   // Deallocates the record data of the table that is linked to the BplusHashMap Bplus-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Drops - removes all the Bplus-tree nodes and records of the BplusHashMap.
*
*  Return value:
*
*  value: 0 - deletion is successful
*  value: 1 - deletion is unsuccessful
*
*/

bool BplusHashMapClean(

        BplusHashMap* BplusHashMap,                     // BplusHashMap index structure object.

        bool (*Deallocate_Record_Data)(BplusTreeData)   // Deallocates the record data of the table that is linked to the BplusHashMap Bplus-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  DJB2 hash algorithm generates a  hash id from a string primary key fields.
*
*  Return value:  Hash code.
*
*/

int BplusTree_DJB2_Hash_PrimaryKeyStr(

        char* record_primary_key,   // Primary key field.
        int hashmap_level_size      // Set of BHashMap B-tree index structures.

);

