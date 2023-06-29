# Pipe (RTL Like) Example

This is a basic example to show how to use SystemC.
Could use it as a template to create more complex models.

This Example is very RTL like.

## Design
 
It consists of 3 processes that form individual stages of a pipleline.

```diagram
| +-----------+     +-----------+   +-----------+  |
| |           |     |           |   |           |  |
| | stage1    |---->| stage2    |-->| stage3    |  |
| |           |     |           |   |           |  |
| +-----------+     +-----------+   +-----------+  |
 ```
> Pipe Diagram

The first stage of the pipeline accepts 2 inputs and computes their sum
and difference.  The second stage accepts the results of the first
stage and computes their product and quotient.  Finally stage3 
accepts these outputs from second stage and computes the first input raised to
the power of the second.