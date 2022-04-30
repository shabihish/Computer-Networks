import re
import numpy as np
import matplotlib.pyplot as plt


def get_truncated_tr_lines(tr_lines: list):
    out = list()
    for i, line in enumerate(tr_lines):
        if re.search('^[sr].*-It tcp.*', line):
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
        delays = list()
        pck_cnt = 0

        for i, line_i in enumerate(self.truncated_tr_lines):
            result_i = re.search('^s -t ([0-9]*[.]?[0-9]*).*-Hs [03].*-Ii (\d+) .*', line_i)
            if result_i:
                pck_cnt += 1
                for j, line_j in enumerate(self.truncated_tr_lines[i + 1:]):
                    result_j = re.search('^r -t ([0-9]*[.]?[0-9]*).*-Hs [78].*-Ii {} .*'.format(result_i.group(2)),
                                         line_j)
                    if result_j:
                        delays.append(float(result_j.group(1)) - float(result_i.group(1)))
                        # print(float(result_j.group(1)) - float(result_i.group(1)))
                        break

        out_dict = {
            'avg_oneway_delay': sum(delays) / len(delays),
            'avg_transfer_ratio': len(delays) / pck_cnt
        }
        return out_dict

    def get_avg_throughput(self):
        arr_th0 = np.genfromtxt(self.th0_file_path, dtype=np.float64)
        arr_th1 = np.genfromtxt(self.th1_file_path, dtype=np.float64)

        plt.plot(arr_th1[:, 1] + arr_th0[:, 1])
        plt.ylim([0, 300000])
        plt.show()
        return np.r_[arr_th0, arr_th1][:, 1].mean()


if __name__ == '__main__':
    parser = TrParser('wireless.tr', 'throughput0.tr', 'throughput1.tr')

    print('Now running...\nThis may take a few moments. Please be patient...')
    # packet_stats = parser.parse_tr_for_packet_statistics()
    avg_throughput = parser.get_avg_throughput()

    print('Avg throughput: {} bits/sec'.format(avg_throughput))
    # print('Avg end-to-end (one-way) delay: {} secs'.format(packet_stats['avg_oneway_delay']))
    # print('Avg transfer ratio: {}'.format(packet_stats['avg_transfer_ratio']))
