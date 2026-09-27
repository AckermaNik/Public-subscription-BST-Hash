# Public Subscription System — Indexed and Prunable Version

This C project extends a group-based subscription system with timestamp pruning and data structures designed for different access patterns. Information is indexed per group in binary search trees; subscribers are indexed in a hash table and also linked from each group; and each subscriber keeps per-group trees and cursors for information moved out of the main store by pruning.

## Data structures

### Fixed group directory

The global `G` array contains 64 `Group` records, addressed directly by group ID (`0–63`). A group stores its ID, a root pointer `gr` for its information tree, and a head pointer `gsub` for its subscriber list. This makes group lookup constant-time by ID.

### Per-group information BST

Each group stores `Info` records in a binary search tree. An `Info` node contains an information ID, its timestamp, and `ilc`, `irc`, and `ip` pointers for its left child, right child, and parent. The tree is ordered by timestamp, supporting ordered traversal and locating/removing information during pruning. This is an ordinary BST, not a self-balancing tree, so its height depends on insertion order. The `igp[64]` array records the groups associated with the information item.

### Subscriber hash table and group indexes

Subscriber records (`SubInfo`) are stored in a hash table with `m` buckets. Registration computes a bucket using `Universal_Hash(m, p, sId)`; collisions are represented by a singly linked chain using `SubInfo.snext`. Each record stores the subscriber ID and registration timestamp.

There is also a separate singly linked `Subscription` list in each group. Its nodes contain subscriber IDs, so the system can enumerate subscribers interested in a group. These are two complementary indexes: the hash table stores subscriber state, while each group's list supports group-to-subscriber traversal, particularly during pruning.

### Per-subscriber, per-group trees and cursors

Every `SubInfo` has two arrays of 64 pointers to `TreeInfo`:

- `tgp[group]` is the subscriber's accumulated tree of information transferred during pruning for that group.
- `sgp[group]` is the subscriber's current consumption position in that tree.

A sentinel pointer marks groups the subscriber did not subscribe to; a null pointer represents a subscribed group that has no stored tree yet. Keeping these states separate lets the program distinguish “not subscribed” from “subscribed, but no pruned information available.”

`TreeInfo` is a leaf-oriented binary search tree containing information IDs and timestamps. Its `tlc`, `trc`, and `tp` fields link the tree structure; `next` and `prev` link its leaves in timestamp order. The tree supports ordered organization, while the leaf-level doubly linked chain lets `Consume` advance to the newest available entry without repeatedly searching the whole tree. This is a different structure from the per-group `Info` BST: the group tree is the active store, while `TreeInfo` holds pruned information for subscribers.

### Why the structures work together

The design keeps one active information tree per group rather than duplicating each item into every subscriber's state. When pruning, the program visits a group's subscribers, copies eligible information (timestamps up to the cutoff) into each interested subscriber's `tgp` tree, updates that subscriber's `sgp` cursor, and then removes those records from the group's active `Info` tree. `Consume` reads and advances the subscriber's cursor through their per-group `TreeInfo` leaves. `Delete_Subscriber` removes the subscriber record from the hash bucket and removes the subscriber ID from every relevant group list.

## Event-file commands

The executable reads one command per line:

```text
I <timestamp> <info_id> <group_id>... -1
S <timestamp> <subscriber_id> <group_id>... -1
R <timestamp_cutoff>
C <subscriber_id>
D <subscriber_id>
P
```

`I` inserts an item into the listed groups; `S` registers a subscriber for groups; `R` prunes information at or before the cutoff; `C` consumes the subscriber's available information; `D` removes a subscriber; and `P` prints the structures. Lines beginning with `#` are ignored. The group ID range is `0–63`.

## Build and run

The command-line arguments are hash-table size `m`, hashing parameter `p`, and the event file:

```sh
gcc -ansi main.c pss.c -o pss
./pss 101 1009 simple_inserts_with_prune_consume.txt
```

The included text files exercise basic, mixed, deletion, pruning/consumption, and larger event sequences. Choose positive values for `m` and `p`; subscriber registration also expects IDs within the supported range implied by `p`.

## Project files

- `main.c` parses command-line arguments and event lines, initializes the structures, dispatches operations, and releases resources.
- `pss.c` implements group information BSTs, subscriber hash buckets and group lists, leaf-oriented subscriber trees, event operations, printing, and cleanup.
- `pss.h` declares the node structures, constants, and PSS operation interfaces.
- `*.txt` files are sample event sequences.
- `.project`, `.cproject`, `.settings/`, and `Debug/` contain Eclipse CDT configuration or generated build artifacts. The checked-in `run.exe` is a prebuilt binary and may not match the source or target platform.
