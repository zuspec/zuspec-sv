
- Class for actor -- derives from some base, some interface, etc
- Base 'action' class
- 
-> Generates two packages
  - implementation package with classes to represent everything
    - Only imported by public package
  - public package that exposes Actor entry-point + public API


- Top-level generator
- IOutput
- Similar strategy around 

- Generate actor structure
- Generate component structure
- Generate action and exec structure
- Evaluate exec structure


Strategy
========
- Focus on a very limited PSS language subset initially
  - Support as natively executable in SystemVerilog (use Verilator as baseline)
    - Use SystemVerilog flavor as lead in explaining/exploring
  - Support as natively executable in C
  - Wrap C flavor to support Python

- Expand supported language subset in the SystemVerilog flavor 
  for full SystemVerilog simulators

Subset Language Features
========================

Candidate 1
***********
- Components, actions
- Sequential activities
- Exec blocks
- External functions
- No constraints.

Candidate 2
***********
- Register model
- No inline constraints
- Activities
- Exec blocks
- No hierarchical constraints, except on address claims
- Single-variable domain constraints

