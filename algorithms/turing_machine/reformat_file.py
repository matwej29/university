def reformat():
    with open("task.txt", 'r') as f:
        lines = f.readlines()

    result = []

    for i, line in enumerate(lines):
        if "{" not in line:
            continue
        else:
            lines = lines[i + 1:]
            break

    for i, line in enumerate(lines):
        if ";" not in line: continue
        result.append(line.split(';')[0].strip()+';')
        if "}" in line: break

    result = '\n'.join(result)

    with open("program.txt", 'w') as f:
        f.writelines(result)


if __name__ == '__main__':
    reformat()
