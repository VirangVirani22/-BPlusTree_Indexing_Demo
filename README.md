# B+ Tree Advanced Data Structure Innovative Assignment
<h2>Project Description</h2>
<p>Implemented using C++</p>
<h2>Demo of Project</h2>


https://user-images.githubusercontent.com/74756979/166731385-9ba0af5f-edc9-4887-aaac-62e2fdb55cf8.mp4


<h3>Order of B+ Tree</h3> 
<p>Order of m B+ Tree can have<br>
<ul>
  <li>Atmost m children</li>
  <li>Atleast ceil[m/2] chirdren</li>
</ul>
  Any node that has m children has m-1 elements.
</p>
<h3>Properties of B+ Tree</h3>
<p>All leaves are at the same level.<br>
The root has at least two children.<br>
All keys in node are in sorted order.<br>
Insertion of a Node in B-Tree happens only at Leaf Node.<br>
All Leaves are connected sequentially.<br>
Data only in leaf nodes
</p>
<h4>You can read more about B+ Tree here : https://www.studytonight.com/advanced-data-structures/b-plus-trees-data-structure</h4>
<h2>Functionality</h2>
<ol>
  <li>Insertion</li>
  <li>Display</li>
    <ul>
      <li>Hierarchical</li>
      <li>Sequential</li>
  </ul>
  <li>Delete</li>
</ol>

<h2>Sample B+ Tree</h2>
<p align="center"><img src="https://github.com/GauravSakariya99/B-Plus-Tree-Advanced-Data-Structure-Innovative-Assignment/blob/main/Image_of_B+Tree.png" align="center"/></p>
<h2>Working of Project</h2>
<p>This project is B+ Tree implementation like small version actual database that uses B+ tree for indexing.in In B+ Tree Internal nodes we store only Keys (Roll Nos) for sorting  and does not contain any data and Leaf Nodes we store Key Value pair (Roll No. , *File Pointer ) . this file pointer points to the actual file where corresponding data to roll no is present. Leaf nodes are connected sequentially.<br><br>

Whenever we insert data the file RollNo.txt(Ex . 233.txt ) will be created in DATA/ folder and has content of name, semester, CGPA. it is same as disc – block access.
In searching we will get node containing roll no. and through its File* we can access file and print its data.<br><br>
  
Delete will delete leaf node(key as RollNo) as well as rollno.txt file from DATA /  folder.
We are making right biased tree if Node is full then after splitting right children will have one more element than left.<br><br>
Here We are inserting some data from students.txt file for easy execution. Then we can do operations manually as we want to do.
</p>
