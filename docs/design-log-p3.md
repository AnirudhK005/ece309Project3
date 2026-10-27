# Project 3 Design Log
**Author:** Anirudh Kopparthi · **P2 code used:** instructor reference
**Tools used:** I used Claude Sonnet for clarification regarding calc_evaluate and the tests.

## 1. Hash table
I used open addressing because it made more sense given that there was no need for removal, which is what chaining is advantageous for. The resize policy 2 * (count + 1) > bucket.size() means that when adding a new tool makes the number of total tools take up greater than 50% of the slots, a resize happens. The offset basis is 1469598103934665603. For each character in the name, it is XORed bitwise with the current hash value and then multiplied with the FNV prime 1099511628211. This yields a number that is then modulo with buckets.size() to obtain a number from 0 to the size - 1, so it will fit somewhere. The resize doubles the table and then replaces the tools. My 10000 tool numbers are bucket_count=32768 load_factor=0.305176 max_probe_length=7. Add is amortized O(1) because each resizing add is O(n), but it rapidly begins to occur less frequently, as the size grows, it occurs once half of the slots fill up. As the size grows, it takes longer and longer for that to happen, so it becomes O(1).

## 2. Pointer stability
<!-- Why does a Tool* returned by find() survive a resize in your design? -->


## 3. Calculator trace
<!-- Trace the values and ops stacks for 2 * (3 + 4) - 5, one token at a
     time, in your own words. -->

First, push 2 to values, push * to ops, push ( to ops, push 3 to values, push + to ops, push 4 to values. Then, calculate 3 + 4 to get 7 then pop (. Then, perform 2 * 7 to get 14. Push - to ops then push 5 to values. Calculate 14 - 5 to get 9.  

## 4. Errors as values
<!-- Why do tools, resolve() and calc_evaluate() return errors instead of
     throwing? When IS an exception the right choice? -->

Returning an error is good because the model or agent may need to attempt something else. An exception would end the run or loop. If a situation makes any further attempt or continuation unnecessary or risky then an exception is needed.

## 5. What I would design differently
I would make a helper function for resizing and remove the resize portion of code from add and have it in this helper function which add can then call.
