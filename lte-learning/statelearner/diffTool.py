import argparse
import os



def diff(dir_statelearner, dir_simulator):
    
    statelearner_queries = []
    simulator_queries = []
    ################################# LEARNING QUERIES ############################
    print '################################# LEARNING QUERIES ############################'
    file_sl_learning_queries_log = dir_statelearner + 'learning_queries.log'
    file_sim_learning_queries_log = dir_simulator + 'learning_queries.log'

    f1 = open(file_sl_learning_queries_log, "r")
    lines_f1 = f1.readlines()
    os.chdir("../")
    f2 = open(file_sim_learning_queries_log, "r")
    lines_f2 = f2.readlines()
    
    file1 = []
    file2 = []
    

    for i in range(0,len(lines_f1)) :
        if 'INFO' in lines_f1[i]:
            line = lines_f1[i].strip()
            file1.append(line)
            if line not in statelearner_queries:
                statelearner_queries.append(line)

    for i in range(0, len(lines_f2)):
        if 'INFO' in lines_f2[i]:
            line = lines_f2[i].strip()
            file2.append(line)
            if line not in simulator_queries:
                simulator_queries.append(line)

    print 'Total queries from Statelearner = ', len(file1)
    print 'Total queries from SIMULATOR = ', len(file2)    

    count = 1
    for i in range(min(len(file1),len(file2))):
        if str(file1[i]) not in str(file2[i]):
            print "First mismatch at Query number: " + str(count)
            print "From Statelearner: " + file1[i]
            print "From Simulator:    " + file2[i]
            raw_input("Press enter to continue\n")
            count = count + 1
            continue
        else:
            count = count + 1

    ################################# EQUIVALENCE QUERIES ############################
    '''
    print '################################# EQUIVALENCE QUERIES ############################'
    os.chdir("./statelearner")
    file_sl_equiv_queries_log = dir_statelearner + 'equivalence_queries.log'
    file_sim_equiv_queries_log = dir_simulator + 'equivalence_queries.log'
    f1 = open(file_sl_equiv_queries_log, "r")
    lines_f1 = f1.readlines()
    os.chdir("../")
    f2 = open(file_sim_equiv_queries_log, "r")
    lines_f2 = f2.readlines()
    
    file1 = []
    file2 = []

    # print len(lines_f1)
    # print len(lines_f2)
    for i in range(0,len(lines_f1)) :
        if 'INFO' in lines_f1[i]:
            line = lines_f1[i].strip()
            file1.append(line)
            if line not in statelearner_queries:
                statelearner_queries.append(line)

    for i in range(0, len(lines_f2)):
        if 'INFO' in lines_f2[i]:
            line = lines_f2[i].strip()
            file2.append(line)
            if line not in simulator_queries:
                simulator_queries.append(line)

    print 'Total queries from Statelearner = ', len(file1)
    print 'Total queries from SIMULATOR = ', len(file2)    
    count = 1
    for i in range(min(len(file1),len(file2))):
        if str(file1[i]) not in str(file2[i]):
            print "First mismatch at Query number: " + str(count)
            print "From Statelearner: " + file1[i]
            print "From Simulator:    " + file2[i]
            break
        else:
            count = count + 1

    #########################################################################

    print '##################################################################\n'
    for sl_query in statelearner_queries:
        if sl_query not in simulator_queries:
            print sl_query
    '''
    

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Diff Tool for Log Files!")
    #parser.add_argument('-file_from_statelearner', dest='dir_statelearner', default="logs/",help="directory to statelearner's output")
    parser.add_argument('-file_from_statelearner', dest='dir_statelearner', default="/home/cyber2slab/Documents/LTEUE-State-Fuzzing/statelearner/logs/",help="directory to statelearner's output")
    parser.add_argument('-file_from_simulator', dest='dir_simulator', default="TLS-Statelearner/statelearner/output/",help="directory to lte ue simulator's output")
    args = parser.parse_args()
    diff(args.dir_statelearner, args.dir_simulator)