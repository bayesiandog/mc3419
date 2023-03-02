# MC3419 Driver
![image](https://www.google.com/url?sa=i&url=https%3A%2F%2Fwww.digikey.com%2Fen%2Fproducts%2Fdetail%2Fmemsic-inc%2FMC3419%2F15292803&psig=AOvVaw0cREtBtGYBQZ-s1-m_K-BF&ust=1677869633671000&source=images&cd=vfe&ved=0CBAQjRxqFwoTCPiB7M71vf0CFQAAAAAdAAAAABAg)

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

