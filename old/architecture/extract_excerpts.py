#!/usr/bin/env python3

if __name__ == '__main__':
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('id', type=str)
    parser.add_argument('infile', type=argparse.FileType('r'), nargs='+')
    args = parser.parse_args()

    for f in args.infile:
        with f:
            marker = 'BEGIN EXCERPT {}'.format(args.id)
            for line in f:
                if marker in line:
                    break

            marker = 'END EXCERPT {}'.format(args.id)
            for line in f:
                if marker in line:
                    break
                print(line.rstrip())

