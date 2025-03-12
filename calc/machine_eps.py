def find_machine_epsilon():
    epsilon = 1.0
    while (1.0 + epsilon) != 1.0:
        epsilon /= 2.0
    return epsilon * 2.0

machine_epsilon = find_machine_epsilon()
print(f"Машинный эпсилон: {machine_epsilon}")
