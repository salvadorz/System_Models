This is a basic example to show how to use SystemC.
can use it as a template to create more complex models.

# Fifo Example

This Example doesn't burn any time but it implements the notion of processes.

## Design  
```diagram 
| +-----------+     +-----------+   +-----------+  |
| |           |---->|           |   |           |  |
| | producer  |     | FIFO      |   | consumer  |  |
| |           |     |           |-->|           |  |
| +-----------+     +-----------+   +-----------+  |
``` 
