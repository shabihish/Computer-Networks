import re
import numpy as np
import matplotlib.pyplot as plt

def get_truncated_tr_lines(tr_lines: list):
    out = list()
    for i, line in enumerate(tr_lines):
        if re.search('^[dsr].*-It tcp.*', line):
            out.append(line)
    return out

class TrParser:
    def __init__(self, tr_file_path: str, th0_file_path: str, th1_file_path: str):
        self.tr_file_path = tr_file_path
        with open(tr_file_path) as f:
            self.tr_lines = f.readlines()

        self.th0_file_path = th0_file_path
        self.th1_file_path = th1_file_path

        self.truncated_tr_lines = get_truncated_tr_lines(self.tr_lines)

    def parse_tr_for_packet_statistics(self):
        delays = dict()
        packets_dict = dict()
        packets_sent = 0

        for i, line_i in enumerate(self.truncated_tr_lines):
            result_srd = re.search('^([srd]) -t ([0-9]*[.]?[0-9]*) .*-Hs (\d+) .*-Ii (\d+) .*', line_i)
            result_dict = {
                'ptype': result_srd.group(1),
                'time': result_srd.group(2),
                'Hs': result_srd.group(3),
                'uid': result_srd.group(4),
            }

            if not result_srd:
                continue

            if result_dict['ptype'] == 's':
                # if the sender's not a tcp node, just ignore the case (tcp nodes: 0, 3)
                if not re.search('[03]', result_dict['Hs']):
                    continue

                if result_dict['uid'] not in packets_dict:
                    packets_dict[result_dict['uid']] = result_dict['time']
                    packets_sent += 1

            if result_dict['ptype'] == 'r':
                # if the receiver's not a sink node, just ignore the case (sink nodes: 7, 8)
                if not re.search('[78]', result_dict['Hs']):
                    continue

                # If packet's not yet sent, there's potentially been an error
                if not result_dict['uid'] in packets_dict:
                    continue

                send_time = float(packets_dict[result_dict['uid']])
                delays[result_dict['uid']] = (float(result_dict['time']) - send_time)

            elif result_srd.group(1) == 'd':
                # If packet's not yet sent, there's potentially been an error
                if not result_dict['uid'] in packets_dict:
                    continue

                packets_dict.pop(result_dict['uid'])
                if result_dict['uid'] in delays:
                    delays.pop(result_dict['uid'])

        out_dict = {
            'avg_oneway_delay': sum(delays.values()) / len(delays),

            # len(delays) is the number of packets received
            'avg_transfer_ratio': len(delays) / packets_sent
        }

        return out_dict

    def get_avg_throughput(self):
        arr_th0 = np.genfromtxt(self.th0_file_path, dtype=np.float64)
        arr_th1 = np.genfromtxt(self.th1_file_path, dtype=np.float64)
        return (arr_th0[:, 1] + arr_th1[:, 1]).mean()
