
- Generator creates model-specific interface and gasket (?)
  - Allow clients to implement any of the exposed functions
  - Base interface issues fatal if not implemented
  - Actor gasket works for any component/action combination

- How are files specified to package?
  - In Python, can specify FuseSoC component identifier
  - Must specify filelist (created by FuseSoc) for SV?

- Single set of files are loaded for full simulation
- Multiple 'actors' can be created that use this database

- Each actor has
  - Instance name 
    - provided by the user
    - Template name specialized
  - Component/Action type combination

- Global functions
  - Fatal error signaling
  - Internal hook to channel messages to environment (does Verilator support?)
    - Looks like vpi_printf works generally
  - Should be able to process PSS source either on-demand (default) or up-front (special plusarg)

- Actor is the internal model that executes the description
- Gasket implements some portions of the Backend API
  - 
- Gasket registers ID to function names
  - Each actor has its own map of function to ID

- SV has similar ValRef wrappers for operating on 
  - Pass ValRef by handle into SV?


0: Initialize core data model from PSS filelist

1: Hand-coded gasket for simple cases

2: Generate gasket

3: Plugin scheme for extending Zuspec



