{- Note [Function order]
~~~~~~~~~~~~~~~~~~~~~~
    Functions in Haskell don't have to be in any particular order, 
    so it doesn't matter if you define doubleMe first 
    and then doubleUs or if you do it the other way around.
-}

-- call func
doubleUsX x y = doubleMe x + doubleMe y

-- my first hs func
doubleMe x = x + x  

-- multi-parameter
doubleUs x y = x*2 + y*2

-- if statement
doubleSmallNumber x = if x > 100  
                        then x  
                        else x*2  

-- apostrophe
doubleSmallNumber' x = (if x > 100 then x else x*2) + 1 

{- List
    We can use the let keyword to define a name right in GHCI. 
    Doing let a = 1 inside GHCI is the equivalent of 
    writing a = 1 in a script and then loading it.
 -}
lostNumbers = [4,8,15,16,23,42] 

-- index
entry = lostNumbers !! 0

-- cycle and take
myList = take 10 (cycle [1,2,3]) 

-- sum statement
mySum = sum myList 

