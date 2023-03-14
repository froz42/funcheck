import { Builder } from './builder';
import childProcess from 'child_process';

export class Runner {
  /**
   * @param args - Arguments to pass to the program
   *
   * @returns The output of the program
   */
  public run(program: string, args: string[]): string {
    const child = childProcess.spawnSync(program, args, {
      cwd: process.cwd(),
    });

    return child.stdout?.toString() || '';
  }
}
