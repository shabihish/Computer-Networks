import matplotlib.pyplot as plt

from parser import TrParser
from subprocess import DEVNULL, STDOUT, check_call

TCL_FILE = 'wireless.tcl'

if __name__ == '__main__':


    n_err_rate_trials = 10
    bandwidths = [1.5, 55, 155]

    for i, BW in enumerate(bandwidths):
        next_err_rate = err_delta = 0.000001
        err_rates = []
        throughputs = []
        delays = []
        transfer_ratios = []

        print(f'Running for {"BW = "+str(BW)}...')
        for j in range(n_err_rate_trials):
            err_rate = next_err_rate
            next_err_rate += err_delta

            print(f'\tNow Running for {"err_rate = "+str(err_rate)}...')
            check_call(['ns', TCL_FILE, str(err_rate), str(BW)], stdout=DEVNULL, stderr=STDOUT)
            print('\tns finished successfully')
            curr_parser = TrParser('wireless.tr', 'throughput0.tr', 'throughput1.tr')
            packet_stats = curr_parser.parse_tr_for_packet_statistics()
            avg_throughput = curr_parser.get_avg_throughput()

            err_rates.append(err_rate)
            throughputs.append(avg_throughput)
            delays.append(packet_stats["avg_oneway_delay"])
            transfer_ratios.append(packet_stats['avg_transfer_ratio'])

            print(f'\tStats: {"avg_throughput = "+str(avg_throughput):<30}, {"avg_end_to_end_delay = "+str(packet_stats["avg_oneway_delay"]):<35}s, {"avg_transfer_ratio = "+str(packet_stats["avg_transfer_ratio"]):<30}')
            print('\n')

        plt.figure(figsize=(15, 6))
        plt.suptitle('Stats for BW: {}Mb/s'.format(BW))

        plt.subplot(1, 3, 1)
        plt.plot(err_rates, throughputs)
        plt.title('Avg Throughput')
        plt.ylabel('average throughput [b/s]')
        plt.xlabel('error rate')

        plt.subplot(1, 3, 2)
        plt.plot(err_rates, transfer_ratios)
        plt.title('Avg Transfer Ratio')
        plt.ylabel('average transfer ratio')
        plt.xlabel('error rate')

        plt.subplot(1, 3, 3)
        plt.plot(err_rates, delays)
        plt.title('Avg End-to-End Delay')
        plt.ylabel('average end-to-end delay [s]')
        plt.xlabel('error rate')

        plt.show()