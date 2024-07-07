from fsmdot.dfa import Dfa

S = {"init", "ike_sa_init_request", "ike_auth_1_request", "ike_auth_2_request", "ike_auth_3_request", "register_1", "register_2", "registered"}
I = {"timeout", "turn_on_wifi", "turn_off_wifi", "ike_sa_init_response", "ike_auth_1_response", "ike_auth_2_response", "ike_auth_3_response", "401_unauthorized", "200_ok"}

T = {}

T["init"] = {}
T["init"]["turn_on_wifi"] = "ike_sa_init_request"

T["ike_sa_init_request"] = {}
T["ike_sa_init_request"]["ike_sa_init_response"] = "ike_auth_1_request"
T["ike_sa_init_request"]["timeout"] = "ike_sa_init_request"
T["ike_sa_init_request"]["turn_off_wifi"] = "init"

T["ike_auth_1_request"] = {}
T["ike_auth_1_request"]["ike_auth_1_response"] = "ike_auth_2_request"
T["ike_auth_1_request"]["timeout"] = "ike_auth_1_request"
T["ike_auth_1_request"]["turn_off_wifi"] = "init"

T["ike_auth_2_request"] = {}
T["ike_auth_2_request"]["ike_auth_2_response"] = "ike_auth_3_request"
T["ike_auth_2_request"]["timeout"] = "ike_auth_2_request"
T["ike_auth_2_request"]["turn_off_wifi"] = "init"

T["ike_auth_3_request"] = {}
T["ike_auth_3_request"]["ike_auth_3_response"] = "register_1"
T["ike_auth_3_request"]["timeout"] = "ike_auth_3_request"
T["ike_auth_3_request"]["turn_off_wifi"] = "init"

T["register_1"] = {}
T["register_1"]["401_unauthorized"] = "register_2"
T["register_1"]["timeout"] = "register_1"
T["register_1"]["turn_off_wifi"] = "init"

T["register_2"] = {}
T["register_2"]["200_ok"] = "registered"
T["register_2"]["timeout"] = "register_2"
T["register_2"]["turn_off_wifi"] = "init"

S0 = "init"

F = {"registered"}

a = Dfa(S, I, T, S0, F)
G = a.dot_graph()
G.write("fsm.dot")
