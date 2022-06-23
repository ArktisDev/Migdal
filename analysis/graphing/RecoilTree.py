import anytree
import pandas as pd

events = recoils[(recoils['Event ID'] == 6411)]
true_recoils = events[~events['Parent ID'].isin(events['Track ID'])]

print('True recoils:')
display(true_recoils)

tree = anytree.Node('6411')

for parent in true_recoils['Parent ID']:
    anytree.Node(str(parent), parent=tree)

for index, recoil in events.iterrows():
    parentID = recoil['Parent ID']
    trackID = recoil['Track ID']
    energy = recoil['Energy (keV)']
    parentNode = anytree.findall(tree, filter_=lambda node: node.name in (str(parentID)))
    
    if(len(parentNode) < 1):
        # Parent is missing, create it
        print('Parent ' + str(parentID) + ' missing')
        parentNode = (anytree.Node(str(parentID), parent=tree),)
    
    # Parent exists now, bind to it
    anytree.Node(str(trackID), parent=parentNode[0], energy = energy)
    
print(anytree.RenderTree(tree, style=anytree.AsciiStyle()))