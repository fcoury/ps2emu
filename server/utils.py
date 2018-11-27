def chomp(x):
    if x.endswith("\r\n"): return x[:-2]
    if x.endswith("\n") or x.endswith("\r"): return x[:-1]
    return x

def split(s, n):
    if len(s) < n:
        return []
    else:
        return [s[:n]] + split(s[n:], n)
