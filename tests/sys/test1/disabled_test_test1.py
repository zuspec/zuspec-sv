

def test_test1(dirconfig):
    print("test_test1")
    sim = pfv.FlowSim(dirconfig)
#    sim.addFileset()

    sim.run_all()

def test_test1_1():
    """This test tests nothing interesting"""
    print("test_test1")
