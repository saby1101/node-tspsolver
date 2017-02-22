export declare interface Options {
    N: number
    T: number
    lambda: number
    reheatInterval: number
}

export declare const solveTsp:
    (costMatrix: number[][], roundtrip: boolean, options?: Options) => Promise<number[]>
