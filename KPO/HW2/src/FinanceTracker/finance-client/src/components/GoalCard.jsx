import React from "react";
import { Card, CardContent, LinearProgress, Stack, Typography } from "@mui/material";

export default function GoalCard({ goal, accountBalance }) {
    const progress = Math.min(100, Math.max(0, (accountBalance / goal.targetAmount) * 100));
    return (
        <Card variant="outlined">
            <CardContent>
                <Typography variant="h6">{goal.name}</Typography>
                <Typography variant="body2" color="text.secondary" sx={{ mb: 1 }}>
                    Дедлайн: {goal.deadline}
                </Typography>
                <Stack direction="row" justifyContent="space-between">
                    <Typography variant="body2">Накоплено: {accountBalance.toFixed(2)} ₽</Typography>
                    <Typography variant="body2">Цель: {goal.targetAmount.toFixed(2)} ₽</Typography>
                </Stack>
                <LinearProgress variant="determinate" value={progress} sx={{ my: 1.5 }} />
                <Typography variant="body2" color={progress >= 100 ? "success.main" : "warning.main"}>
                    Прогресс: {progress.toFixed(1)}%
                </Typography>
            </CardContent>
        </Card>
    );
}