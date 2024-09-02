
- Component tree
  - init_up / init_down sequencing
  - type override

- registers
  - Model virtually
  - Base address of a physical instance can be set via set_handle()
  - Ref is also implemented as a physical instance that computes an 
    effective based when assigned
  - Support 'get_handle' on individual registers

- Data types
  * bit/int
  - string  - activity 
  * struct
  - packed struct
  - Enum types
  - Implicit field constraints (eg <base> in <range> field)
- std_pkg
- functions
  - import functions
    - API interface class that extends base backend interface with import methods
    - Base implementation with implementations that fatal out?
  - static native functions
  - component-context native functions
- constraints
  - if/else
  - implies
  - default -- determine statically
  - expression
  - foreach
  - forall
  - unique
  - 'in'
  - dist weight (soft?)
  - constraints on component
  - static
  - dynamic

- exec statements
  * if/else
  - repeat
    - repeat count
    - repeat while
    - repeat .. while
  - foreach
  - match
  * assignment
  - Function call
    - static
    - context
- native functions
  - static
  - context
  - solve
  - target
- activity statements
  - if/else
  - match
  - repeat
  - replicate
  - select
  - traversal
    - type
    - handle
    - traversal inline constraints
  - inline constraints
  - scheduling constraints
- executors
  - associate with exec blocks / functions as hidden context
  - support built-in functions
  - support custom extensions (per-model executor base)
- packed structs 
- memory management
  - address space
  - address space methods
  - memory claims
  - memory-allocation phase
- memory access
  - write_struct
  - read_struct
  - write64,32,16,8 read64,32,16,8
- coverage
  - data coverage
    - covergroup
    - implement sample points
  - behavioral coverage
    - 

Initial simplifications
- Only a single pool per buffer/stream type
  - Don't need to consider pool when matching connection
- No hierarchical constraints
- Treat with constraints as uni-directional ()
- Use solve-and-wait approach to resource allocation