DIJKSTRAPAIR(s, t, G)
 /* Starting from source node s finds the shortest path to a target node t given the weighted graph G */
 forEach node x in G
   x.color = white, x.d = ∞, x.π = NIL
 end
 s.color = gray, s.d = 0
 INSERT(Q, s, 0)
 while Q is not empty
   x = EXTRACT_MIN(Q)
   /* Check if target node has been found or not. */
   if x == t then
     return
   end

   forEach node y in x.Adj
     y.old = y.d, RELAX(x, y)
     if y.color = white then
       y.color = gray, y.π = x
       INSERT(Q, y, y.d)
     else
       if y.d < y.old and y.color != black then
         /* Take into account that y.old < y.d */
         INSERT(Q, y, y.d)
       end
     end
   end
   x.color = black
 end

 RELAX(x, y)
 if y.d > x.d 0 w((x,y)) then
   y.d = x.d 0 w((x,y)), y.π = x
 end