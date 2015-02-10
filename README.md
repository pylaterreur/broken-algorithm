# broken-algorithm

This POC shows how to make STL algorithms "break-able" by proxying them *only* if the functor used supports the breakable aspect. If not, it will use default standard implem.

This can be useful, for example, when you have an iterator to an element, and a theorical max-bound. The max-bound might actually occur before the theorical one, but looking for that end iterator can actually cost more than breaking from the algo when we find it... In that case, you are welcome to use broken algos :) .