# clib - C language miscellaneous tools library

![CI](https://github.com/izuzanak/clib/workflows/CI/badge.svg)

A collection of specialized C libraries and daemon applications, built
around [`cont_c`](https://github.com/izuzanak/cont/tree/cont_c) - a code
generator for type-safe containers (arrays, red-black trees, linked lists,
queues, structs, tagged unions). The libraries cover common infrastructure
needs: networking, storage, parsing, system interfaces, and more.

## Project Structure

```
src/
├── clib/
│   ├── libs/          46 core libraries
│   └── tests/         test suites
├── segmentd/          segment database daemon
├── objectdb/          object database daemon
├── inverteddb/        inverted index daemon
├── watchdogd/         watchdog daemon
└── httprecv/          HTTP receiver daemon
```

### Key Libraries by Category

| Category | Libraries |
|---|---|
| **Core** | `libbase_cll` (containers, strings, memory), `libvar_cll` (dynamic variable system), `libsys_cll`, `libtest_cll` (test framework) |
| **Networking** | `libtcp_cll` (TCP/SSL), `libhttp_cll`, `libmicrohttp_cll`, `libwebsocket_cll`, `libmqtt_cll`, `librtsp_cll`, `libcurl_cll` |
| **Storage** | `libpsql_cll` (PostgreSQL), `libsqlite_cll`, `libsnappy_cll` (compression), `libfuse_cll` / `libfusehl_cll` (FUSE) |
| **Parsing** | `libjson_cll`, `libxml_cll`, `libxsd_cll`, `libregex_cll`, `libpcre_cll`, `libparser_cll` (LALR) |
| **System** | `liblinux_cll` (epoll, sockets, mmap), `libprocess_cll`, `liblogger_cll`, `libtime_cll`, `libtzone_cll`, `libchannel_cll` |
| **Crypto & encoding** | `libcrypto_cll`, `libopenssl_cll`, `libcrc_cll`, `libzlib_cll`, `libisal_cll`, `libutf8proc_cll` |
| **Graphics / HW** | `libopengl_cll`, `libglfw_cll`, `libftgl_cll`, `libimage_cll`, `libcurses_cll`, `libgpsd_cll`, `libquirc_cll` |
| **Other** | `libvalidator_cll`, `libpam_cll`, `libuuid_cll`, `libsrtp_cll`, `libvdv301_cll` |

## Building

### 1. Prerequisites

Install the [`cont_c`](https://github.com/izuzanak/cont/tree/cont_c) code generator and required dependencies.

### 2. Build

```bash
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

### 3. Run tests

```bash
mkdir -p /dev/shm/{run,log}
make test
```

A single test can be run directly:

```bash
./libbase_cll_test all              # run all tests in the suite
./libbase_cll_test bc_array         # run a specific test
```

## Examples

All examples below use `CONT_INIT_CLEAR` which handles both initialization and
cleanup automatically via GCC's `__attribute__((cleanup(...)))`.

### Dynamic Array

```c
CONT_INIT_CLEAR(ui_array_s, array);

// Push 0..9
unsigned idx = 0;
do {
  ui_array_s_push(&array, idx);
} while (++idx < 10);

// Iterate
if (array.used != 0)
{
  ui *ptr = array.data;
  ui *ptr_end = ptr + array.used;
  do {
    printf("value: %u\n", *ptr);
  } while (++ptr < ptr_end);
}
```

### Queue

```c
CONT_INIT_CLEAR(ui_queue_s, queue);

// Enqueue 0..9
unsigned idx = 0;
do {
  ui_queue_s_insert(&queue, idx);
} while (++idx < 10);

// Dequeue all
while (queue.used > 0)
{
  ui value = ui_queue_s_next(&queue);
  printf("value: %u\n", value);
}
```

### Linked List

```c
CONT_INIT_CLEAR(ui_list_s, list);

// Append 0..9
unsigned idx = 0;
do {
  ui_list_s_append(&list, idx);
} while (++idx < 10);

// Iterate
if (list.first_idx != c_idx_not_exist)
{
  unsigned idx = list.first_idx;
  do {
    ui *value_ptr = ui_list_s_at(&list, idx);
    printf("value: %u\n", *value_ptr);
    idx = ui_list_s_next_idx(&list, idx);
  } while (idx != c_idx_not_exist);
}
```

### Red-Black Tree

```c
CONT_INIT_CLEAR(ui_tree_s, tree);

// Insert 0..9
unsigned idx = 0;
do {
  ui_tree_s_insert(&tree, idx);
} while (++idx < 10);

// In-order traversal (smallest to largest)
if (tree.root_idx != c_idx_not_exist)
{
  unsigned idx = ui_tree_s_get_min_value_idx(&tree, tree.root_idx);
  do {
    ui *value_ptr = ui_tree_s_at(&tree, idx);
    printf("value: %u\n", *value_ptr);
    idx = ui_tree_s_get_next_idx(&tree, idx);
  } while (idx != c_idx_not_exist);
}
```

## License

This project is licensed under the [GNU General Public License v2.0](LICENSE).
