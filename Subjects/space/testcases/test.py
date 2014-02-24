f=open("testcases.txt","r")
rs = f.readlines()
f.close()

for r in rs:
        print ("cp /home/yuejia/Projects/GImalloc/test_myga_space/testcases/" + r.strip() + ".adl" + " .")
