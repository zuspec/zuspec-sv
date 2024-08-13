
* Test component init_up / init_down
- registers
  - Model virtually
  - Handle (physical or ref) modeled as address handle (ref+base)

- Data types
  * bit/int
  - string
  * struct
  - packed struct
  - Enum types
  - Implicit field constraints (eg <base> in <range> field)
- std_pkg
- import functions
  - API interface class that extends base backend interface with import methods
  - Base implementation with implementations that fatal out?
- constraints
  - if/else
  - expression
  - foreach
  - unique
  - 'in'

- exec statements
  - if/else
  - repeat
  - foreach
  - match
  - assignment
  - Function call
    - static
    - context
- native functions
  - static
  - context
  - solve
  - target
- activity statements
  - traversal
  - if/else
  - repeat
  - match
  - select
- executors
  - associate with exec blocks / functions as hidden context
  - support built-in functions
  - support custom extensions (per-model executor base)
- packed structs 
- memory management
