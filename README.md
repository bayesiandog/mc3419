# MC3419 Driver
![image](https://user-images.githubusercontent.com/50047346/222524640-d0a7eee4-c5e0-4388-aa93-3b09b20c28ab.png)

## Description

Hash table implementation with separate chaining collision handling.
This means that when a new key maps to an already used index of the table, the new value is also stored in that index.

This is achieved by using a linked list inside the hash table structure and simply adding nodes to each index whenever a collision occurs.

The user can retrieve or delete any node on any index.

## Example

### Create new hash table
```c
ht* htable = ht_create();
```
### Create entry
```c
ht_entry* entry = create_entry(key, value);
```
### Insert entry
```c
insert(entry, htable);
```
### Retrieve entry
```c
retrieve(htable, entry, v);
```
### Delete entry
```c
delete(htable, entry);
```
Run the test example in the main file to try it out.

