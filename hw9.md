The example code contains two functions, map and reduce. The map function takes a key (document name) and a value (document content) as input, and record an associated count of orrurences. The reduce function takes a key (a word) and a value (a list of counts) and sum up all the counts for a specific word. 

Input: 

document name: document 1

document content: "Today is sunny. Tomorrow is rainy. Yesterday is sunny."

Output:

```json
{
  "today": 1
  "is": 3
  "sunny": 2
  "tomorrow": 1
  "rainy": 1
  "Yesterday": 1
}
```

