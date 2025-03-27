# Pushdown-Automata-Simulator

This repository contains a C++ implementation of a Pushdown Automata (PDA) simulator that checks whether given strings are accepted by a Context-Free Grammar (CFG) converted to Greibach Normal Form (GNF). The program processes the CFG, converts it to GNF, and then simulates the PDA to validate input strings.

# Key Features

> CFG To GNF Conversion

Converts the input CFG into Greibach Normal Form (GNF) by:
Eliminating null productions (?).     
Removing unit productions (e.g., A->B).            
Eliminating useless productions.         
Ensuring productions start with a terminal followed by non-terminals.             

> PDA Simulation

 It simulates a PDA to check if input strings are accepted by the converted GNF. It also tests multiple input stringd in a single run.

 # How It Works

> Input CFG

The user provides a CFG in a specific format, such as S->A|b|?, where ? represents a null production. The program reads the number of production lines and the rules themselves.

> Conversion to GNF

The CFG undergoes several transformations to convert it into GNF:

Null Production Removal: The program eliminates any productions that derive the null string.         
Unit Production Elimination: Productions of the form A->B are removed by substituting the non-terminal B with its productions.         

Useless Production Discarding: Productions that do not contribute to generating any terminal strings are discarded.        

GNF Transformation: The grammar is restructured so that all productions start with a terminal followed by non-terminals.         

> String Validation

The PDA simulator uses the GNF-converted CFG to validate input strings. It processes each string by pushing and popping symbols from a stack, following the grammar rules to determine acceptance.

# Limitations
The CFG must adhere to the specified input format. The implementation assumes the input CFG can be converted to GNF. Only deterministic PDAs are supported; ambiguous grammars are not handled.

 
