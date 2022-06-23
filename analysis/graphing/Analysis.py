import uproot
import numpy as np
import pandas as pd

def CountsThroughSurface(eventsDF: pd.DataFrame, tolerance = 1e-8):
    data = []
    surfaces = ['+z', '-z', '+y', '-y', '+x', '-x']
    
    particles = eventsDF['Particle Name'].value_counts().keys()
    
    totalcounts = 0
    totalenergies = pd.Series(dtype=np.float64)
        
    for particle in particles:
        filteredDF = eventsDF[eventsDF['Particle Name'] == particle]
        particletotalcounts = 0
        particletotalenergies = pd.Series(dtype=np.float64)
        
        for surface in surfaces:
            axis = 'Position ' + surface[1] + ' (mm)'
            if surface[0] == '+':
                events = filteredDF[abs(filteredDF[axis] - 100.0) <= tolerance]
                event_energies = events['Energy (keV)']
                counts = events.shape[0]
            else:
                events = filteredDF[abs(filteredDF[axis] + 100.0) <= tolerance]
                event_energies = events['Energy (keV)']
                counts = events.shape[0]
            
            particletotalcounts += counts
            particletotalenergies = pd.concat([particletotalenergies, event_energies])
            data.append([surface, particle, counts, event_energies])
        
        totalcounts += particletotalcounts
        totalenergies = pd.concat([totalenergies, particletotalenergies])
        data.append(['Total', particle, particletotalcounts, particletotalenergies])
    
    data.append(['Total', 'Total', totalcounts, totalenergies])
    
    df = pd.DataFrame(data, columns=['Surface', 'Particle Name', 'Counts', 'Energies'])
    
    return df
    
    